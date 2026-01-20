#include "app/engine.hpp"

#include <iostream>

int main()
{
    niqqa::NiqqaEngine test_window(800, 600, "RGB Triangle");

    try 
    {
        test_window.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}