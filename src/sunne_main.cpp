/* ---------------------------------------------------------------- *
   Antti Jumpponen <kuumies@gmail.com>
   Main entry point of the sunne application.
 * ---------------------------------------------------------------- */
 
#include <iostream>
#include "sunne_controller.h"

/* ---------------------------------------------------------------- *
   Main entry
 * ---------------------------------------------------------------- */
int main(int argc, char* argv[])
{
    try
    {
        using namespace kuu;
        using namespace kuu::sunne;
        Controller controller;
        controller.run();
    }
    catch(const std::runtime_error& error)
    { std::cerr << error.what(); };

    return EXIT_SUCCESS;
}
