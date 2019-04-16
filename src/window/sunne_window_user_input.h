/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Definition of kuu::sunne::WindowUserInput struct.
 * ---------------------------------------------------------------- */

#pragma once

/* ---------------------------------------------------------------- */

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace kuu
{
namespace sunne
{

/* ---------------------------------------------------------------- *
   An input that user made to single window.
 * ---------------------------------------------------------------- */
struct WindowUserInput
{
    // Status
    enum class Status
    {
        Pressed, // input is pressed down
        Repeat,  // input is hold down
        Release, // input is released
    };

    // Type of the input.
    enum class Type
    {
        Key,    // keyboard input
        Mouse,  // mouse input
        Wheel,  // wheel input
        Cursor, // cursor input
    } type;

    // Key data
    struct KeyData
    {
        int key;
        int status;
        int modifiers;
    } key;

    // Key data
    struct MouseData
    {
        int button;
        int status;
        int modifiers;
        glm::vec2 pos;
    } mouse;

    // Wheel data
    struct WheelData
    {
        glm::vec2 pos;
    } wheel;

    // Cursor data
    struct CursorData
    {
        glm::vec2 pos;
    } cursor;

    // Construct window user input
    WindowUserInput(Type type);
};

} // namespace sunne
} // namespace kuu
