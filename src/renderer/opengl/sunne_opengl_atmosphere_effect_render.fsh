/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   kuu::OpenGLAtmosphereEffectRender fragment shader.
 * ---------------------------------------------------------------- */
 
#version 330 core

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
const float PI                     = 3.14159f;
const vec3 lightDir                = normalize(-vec3(1.0, -1.0, 1.0));
const vec3 lightIntensity          = vec3(20.0, 20.0, 20.0);
const int viewRaySampleCount       = 8;
const int lightRaySampleCount      = 4;
const float planetRadius           = 6360;
const float atmosphereRadius       = 6420;
const float rayleighScaleHeight    = 7.994;
const float mieScaleHeight         = 1.200;
const vec3 rayleighScatteringCoeff = vec3(3.8e-4, 13.5e-4, 33.1e-4);
const vec3 mieScatteringCoeff      = vec3(21e-3);

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
uniform vec2 viewport;
uniform mat4 inverseView;
uniform mat3 inverseTransposeView;
uniform mat4 inverseProjection;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct Ray
{
    vec3 origo;
    vec3 direction;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct Sphere
{
    vec3 center;
    float radius;
};

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
struct RaySphereIntersectionResult
{
    bool hit;
    float tNear;
    float tFar;
};

/* ---------------------------------------------------------------- *
   Unprojects a viewport position into world space ray.
 * ---------------------------------------------------------------- */
Ray fragmentRay(vec2 fragmentPos,
                vec2 viewport,
                mat4 inverseView,
                mat3 inverseTransposeView,
                mat4 inverseProjection)
{
    // [vp] -> [0, 1], note that y is flipped
    vec2 fragCoord = fragmentPos.xy / viewport.xy;
    fragCoord.y = 1.0 - fragCoord.y;

    // [0, 1] -> [-1, 1] (NDC)
    fragCoord = (fragCoord - 0.5) * 2.0;

    // [NDC] -> [view]
    vec4 deviceNormal = vec4(fragCoord, 0.0, 1.0);
    vec3 eyeNormal = normalize(vec3(inverseProjection * deviceNormal));

    // [view] -> [world]
    vec3 worldNormal = normalize(inverseTransposeView * eyeNormal);;

    // [view] -> [world]
    Ray viewRay;
    viewRay.origo     = vec3(inverseView * vec4(0, 0, 0, 1));
    viewRay.direction = worldNormal;
    return viewRay;
}

/* ---------------------------------------------------------------- *
   Calculates an intersection result between a ray and sphere.
   See Figure 1 from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
 * ---------------------------------------------------------------- */
RaySphereIntersectionResult raySphereIntersection(const Ray ray, const Sphere sphere)
{
    RaySphereIntersectionResult res;
    res.hit   = false;
    res.tNear = 0;
    res.tFar  = 0;

    // Project origo-to-center vector into ray direction
    vec3 l = sphere.center - ray.origo;
    float tca = dot(l, ray.direction);

    // a^2 = c^2 - b^2
    float d2 = dot(l, l) - tca * tca;
    float radius2 = sphere.radius * sphere.radius;
    if (d2 > radius2)
        return res;

    float thc = sqrt(radius2 - d2);

    res.tNear = tca - thc;
    res.tFar  = tca + thc;

    if (res.tNear > res.tFar)
    {
        float temp = res.tNear;
        res.tNear = res.tFar;
        res.tFar = temp;
    }

    if (res.tNear < 0.0)
    {
        res.tNear = res.tFar;
        if (res.tNear < 0.0)
            return res;
    }

    res.hit = true;
    return res;
}

/* ---------------------------------------------------------------- *
   Measure the average density of particles at atmosphere between a
   points A and B with a numerical integration. Measurement is done
   for air molecule (Rayleigh) and aerosol (Mie) particles. Returned
   value is an inverse kilometer where x is Rayleigh and y is Mie
   average density.

   density(h) = Sigma(a to b) exp(-h/H) (H -> scale height)

   Average density is used to calculate the extinction coefficients
   of Rayleigh and Mie scrattering.
 * ---------------------------------------------------------------- */
vec2 averageParticleDensity(vec3 a, vec3 b)
{
    int densitySampleCount = 8;
    vec3 ba = b - a;
    vec3 direction = normalize(ba);
    float distance  = length(ba);
    float step = distance / float(densitySampleCount);
    vec2 density = vec2(0.0);
    for (int i = 0; i < densitySampleCount; ++i)
    {
        float t = i * step + step * 0.5;
        vec3 p = a + direction * t;
        float h = length(p) - planetRadius;
        density.x += exp(-h / rayleighScaleHeight);
        density.y += exp(-h / mieScaleHeight);
    }
    return density * step;
}

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
out vec4 outColor;

/* ---------------------------------------------------------------- *
 * ---------------------------------------------------------------- */
void main()
{
    // ---------------------------------------------------------------
    // Light direction is *towards* the light in *world space*.

    vec3 lightDirection = -lightDir;

    // ---------------------------------------------------------------
    // Calculate the view ray in *world space* at this texel.

    Ray viewRay = fragmentRay(gl_FragCoord.xy,
                              viewport,
                              inverseView,
                              inverseTransposeView,
                              inverseProjection);

    // ---------------------------------------------------------------
    // Intersect the view ray with the planet and atmosphere

    Sphere planet;
    planet.center = vec3(0.0, 0.0, 0.0);
    planet.radius = planetRadius;
    RaySphereIntersectionResult viewPlanetRes;
    viewPlanetRes = raySphereIntersection(viewRay, planet);

    Sphere atmosphere;
    atmosphere.center = vec3(0.0);
    atmosphere.radius = atmosphereRadius;
    RaySphereIntersectionResult viewAtmosphereRes;
    viewAtmosphereRes = raySphereIntersection(viewRay, atmosphere);

    // ---------------------------------------------------------------
    // Render space if no hit with atmosphere

    if (!viewAtmosphereRes.hit)
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // ---------------------------------------------------------------
    // Select the near and far distances along the view ray.

    float tNear = 0.0;
    if (viewAtmosphereRes.tNear > 0.0)
        tNear = viewAtmosphereRes.tNear;

    float tFar = 0.0;
    if (!viewPlanetRes.hit)
        tFar = viewAtmosphereRes.tFar;
    else
        tFar = viewPlanetRes.tNear;

    // ---------------------------------------------------------------
    // Raymarching along the view ray

    float viewRaySegment = (tFar - tNear) / viewRaySampleCount;
    vec3 viewStartPosOnRay = viewRay.origo + viewRay.direction * tNear;

    vec3 rayleight = vec3(0.0);
    vec3 mie = vec3(0.0);

    for (int viewIndex = 0; viewIndex < viewRaySampleCount; ++viewIndex)
    {
        float viewRayDistance = viewIndex * viewRaySegment + 0.5 * viewRaySegment;
        vec3 viewRayPos = viewStartPosOnRay + viewRay.direction * viewRayDistance;

        Ray lightRay;
        lightRay.origo     = viewRayPos;
        lightRay.direction = lightDirection;
        RaySphereIntersectionResult lightAtmosphereRes;
        lightAtmosphereRes = raySphereIntersection(lightRay, atmosphere);
        if (!lightAtmosphereRes.hit)
            return;

        vec3 lightRayPos = viewRayPos + lightDirection * lightAtmosphereRes.tNear;

        vec2 densityView  = averageParticleDensity(viewRayPos, viewStartPosOnRay);
        vec2 densityLight = averageParticleDensity(viewRayPos, lightRayPos);
        vec2 densities = densityView + densityLight;

        //                            |---------density --------|
        // T(a, b) = exp(-sigmaE(0) * (sum(a,b) * exp(-h/H) * ds)
        // T(view) * T(light)

        float h = length(viewRayPos) - planetRadius;
        float densityRayleigh = exp(-h / rayleighScaleHeight);
        float densityMie      = exp(-h / mieScaleHeight);

        // integral(a, b) T(a, x) * T(x, l) * scattering_coeff(h) * ds
        //          |----------------T(a,x)----------------------|  |--------------------T(x,l)-------------------| |-scatt_coeff(h)-|  |-----ds-----|
        rayleight += exp(-rayleighScatteringCoeff * densityView.x) * exp(-rayleighScatteringCoeff * densityLight.x) * densityRayleigh * viewRaySegment;
        mie       += exp(-mieScatteringCoeff      * densityView.y) * exp(-mieScatteringCoeff      * densityLight.x) * densityMie      * viewRaySegment;
    }

    float vDotL = dot(-viewRay.direction, lightDirection);
    float g = 0.76f;
    float rayleighPhase = 3.0f / (16.0f * PI) *  (1.0f + vDotL * vDotL);
    float miePhase      = 3.0f / (8.0f  * PI) * ((1.0f - g * g) * (1.0f + vDotL * vDotL)) / ((2.0f + g * g) * pow(1.0f + g * g - 2.0f * g * vDotL, 1.5f));
    //float miePhase      = (3.0 *  (1.0 - g * g) * (1.0 + vDotL * vDotL))  / (2.0 * (2.0 + g * g) * pow(1.0 + g * g - 2.0 * g * vDotL, 1.5));

    vec3 rayleighInScattering = lightIntensity * rayleighScatteringCoeff * rayleight * rayleighPhase;
    vec3 mieInScattering      = lightIntensity * mieScatteringCoeff      * mie       * miePhase;

    outColor.rgb = rayleighInScattering + mieInScattering;
    outColor.a = 1.0;
}
