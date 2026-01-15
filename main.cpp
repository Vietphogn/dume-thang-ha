#include "app/application.hpp"

#include <iostream>

int main()
{
    niqqa::NiqqaRenderer engine;

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