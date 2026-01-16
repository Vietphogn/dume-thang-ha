#include "app/application.hpp"

#include <iostream>

int main()
{
    niqqa::NiqqaRenderer engine(1280, 720, "RGB Triangle");

    try 
    {
        engine.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}