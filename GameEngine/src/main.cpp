#include "Engine.h"

#include <iostream>

int main()
{
    try
    {
        Engine engine;
        engine.run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred." << std::endl;
    }
    
    return 0;
}
