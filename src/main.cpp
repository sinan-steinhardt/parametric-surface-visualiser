#include <glm/glm.hpp>
#include "raylib.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <numbers>
#include <math.h>
#include <cmath>
#include <sstream>
#include <utility>
#include <stdexcept>
#include "muParser.h"

std::pair<float,float> extractBounds(const std::string& range) {
    float a, b;
    char open, comma, close;
    std::stringstream ss(range);

    if (ss >> open >> a >> comma >> b >> close) {
        if (open == '[' && comma == ',' && close == ']' && a < b) {
            return {a, b};
        }
    }
    throw std::invalid_argument("");

}

int main() {
    //Get user info
    mu::Parser parserX;
    mu::Parser parserY;
    mu::Parser parserZ;

    double mu_t = 0.0;
    double mu_l = 0.0;

    parserX.DefineVar("t", &mu_t);
    parserX.DefineVar("l", &mu_l);
    parserY.DefineVar("t", &mu_t);
    parserY.DefineVar("l", &mu_l);
    parserZ.DefineVar("t", &mu_t);
    parserZ.DefineVar("l", &mu_l);

    std::string tRange;
    while (true) {
        std::cout << "Please insert your parameter ranges in the desired format below:" << std::endl;
        std::cout << "for t" << std::endl;
        std::cout << "[a,b] = ... " << std::endl;
        std::getline(std::cin, tRange);
        try {
            extractBounds(tRange); // Test extraction to catch errors
            break; // Exit loop if no exception is thrown
        }
        catch (const std::invalid_argument& error) {
            std::cerr << "Error in t range: " << error.what() << std::endl;
            std::cerr << "Ensure that a < b and both are floats." << std::endl;
        }
    }

    std::string lRange;
    while (true) {
        std::cout << "Please insert your parameter ranges in the desired format below:" << std::endl;
        std::cout << "for l" << std::endl;
        std::cout << "[a,b] = ... " << std::endl;
        std::getline(std::cin, lRange);
        try {
            extractBounds(lRange); // Test extraction to catch errors
            break; // Exit loop if no exception is thrown
        }
        catch (const std::invalid_argument& error) {
            std::cerr << "Error in l range: " << error.what() << std::endl;
            std::cerr << "Ensure that a < b and both are floats." << std::endl;
        }
    }

    float tMax = extractBounds(tRange).second;
    float tMin = extractBounds(tRange).first;
    float lMax = extractBounds(lRange).second;
    float lMin = extractBounds(lRange).first;

    std::cout << "" << std::endl;

    std::string XofTL;
    while (true) {
        std::cout << "Please insert your parametrisations as a function of t and l below:" << std::endl;
        std::cout << "x(t,l) = ... " << std::endl;
        std::getline(std::cin, XofTL);
        try {
            parserX.SetExpr(XofTL);
            int steps = 100;
            for (int i = 0; i<=steps; i++) {

                for (int j = 0; j<=steps; j++) {

                    mu_t = tMin + (tMax - tMin) * i / steps;
                    mu_l = lMin + (lMax - lMin) * j / steps;

                    if (std::isnan(parserX.Eval())) {
                        throw std::runtime_error("Evaluation resulted in NaN");
                    }
                }

            }
            break; // Exit loop if no exception is thrown
        }
        catch (const std::runtime_error& error) {
            std::cerr << "Evaluation error: " << error.what() << std::endl;
            std::cerr << "Please input your parametrisation such that it is defined within the parameter ranges." << std::endl;
        }
        catch (mu::Parser::exception_type& error) {
            std::cerr << "Syntax error: " << error.GetMsg() << std::endl;
            std::cerr << "Please input your parametrisation using muParser expression syntax ( e.g. 2*cos(t) + sin(l) )." << std::endl;
        }
    }

    std::string YofTL;
    while (true) {
        std::cout << "Please insert your parametrisations as a function of t and l below:" << std::endl;
        std::cout << "y(t,l) = ... " << std::endl;
        std::getline(std::cin, YofTL);
        try {
            parserY.SetExpr(YofTL);
            int steps = 100;
            for (int i = 0; i<=steps; i++) {

                for (int j = 0; j<=steps; j++) {

                    mu_t = tMin + (tMax - tMin) * i / steps;
                    mu_l = lMin + (lMax - lMin) * j / steps;

                    if (std::isnan(parserY.Eval())) {
                        throw std::runtime_error("Evaluation resulted in NaN");
                    }
                }

            }
            break; // Exit loop if no exception is thrown
        }
        catch (const std::runtime_error& error) {
            std::cerr << "Evaluation error: " << error.what() << std::endl;
            std::cerr << "Please input your parametrisation such that it is defined within the parameter ranges." << std::endl;
        }
        catch (mu::Parser::exception_type& error) {
            std::cerr << "Syntax errror: " << error.GetMsg() << std::endl;
            std::cerr << "Please input your parametrisation using muParser expression syntax ( e.g. 2*cos(t) + sin(l) )." << std::endl;
        }   
    }

    std::string ZofTL;
    while (true) {
        std::cout << "Please insert your parametrisations as a function of t and l below:" << std::endl;
        std::cout << "z(t,l) = ... " << std::endl;
        std::getline(std::cin, ZofTL);
        try {
            parserZ.SetExpr(ZofTL);
            int steps = 100;
            for (int i = 0; i<=steps; i++) {

                for (int j = 0; j<=steps; j++) {

                    mu_t = tMin + (tMax - tMin) * i / steps;
                    mu_l = lMin + (lMax - lMin) * j / steps;

                    if (std::isnan(parserZ.Eval())) {
                        throw std::runtime_error("Evaluation resulted in NaN");
                    }
                }

            }
            break; // Exit loop if no exception is thrown
        }
        catch (const std::runtime_error& error) {
            std::cerr << "Evaluation error: " << error.what() << std::endl;
            std::cerr << "Please input your parametrisation such that it is defined within the parameter ranges." << std::endl;
        }
        catch (mu::Parser::exception_type& error) {
            std::cerr << "Syntax error: " << error.GetMsg() << std::endl;
            std::cerr << "Please input your parametrisation using muParser expression syntax ( e.g. 2*cos(t) + sin(l) )." << std::endl;
        }
    }

    //Define Point Path, Time, Lime (uses glm::vec3 for glm calculations)
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
    std::vector<glm::vec3> path;
    bool TorL = true;
    bool increasing = true;
    bool firstRun = true;
    float t = tMin;
    float l = lMin;
    float tspeed = (tMax - tMin)/5; //1 seconds to go from min to max
    float lspeed = (lMax - lMin)/5; //1 seconds to go from min to max

    //Define Window
    InitWindow(1000, 800, "3DSurface");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    //Set up Window
    SetTargetFPS(60);

    //Define Camera
    Camera3D camera = {{0}};

    //Set up Camera (uses Vector3 for raylib.h rendering)
    camera.position = Vector3{5.0f, 5.0f, 5.0f};
    camera.target = Vector3{0, 0, 0};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraAngle = 0.0f;
    float cameraRadius = 4.0f;
    float cameraSpeed = 0.08f;
    float cameraZoomSpeed = 0.05f;

    //Running and Rendering
    while(!WindowShouldClose()) {

    if (tMin >= tMax || lMin >= lMax) {
        break;
    }

        //Check if switching between t and l is needed based on its value
        if (TorL == true) {
            //t changes and l is constant
            if (increasing == true && firstRun == true) {
                t += tspeed * GetFrameTime();

                if (t >= tMax) {
                    t = tMax;
                    TorL = false;
                    firstRun = false;
                }
            }

            else if (increasing == true && firstRun == false) {
                t += tspeed * GetFrameTime();

                if (t >= tMax) {
                    t = tMax;
                    TorL = false;
                    lMax -= 2 * lspeed * GetFrameTime();
                }
            }

            else if (increasing == false && firstRun == false) {
                t -= tspeed * GetFrameTime();

                if (t <= tMin) {
                    t = tMin;
                    TorL = false;
                    lMin += 2 * lspeed * GetFrameTime();

                }

            }

        }

        else if (TorL == false) {
            //l changes and t is constant
            if (increasing == true && firstRun == false) {
                l += lspeed * GetFrameTime();

                if (l >= lMax) {
                    l = lMax;
                    TorL = true;
                    increasing = false;
                    tMin += 2 * tspeed * GetFrameTime();
                }
            }

            else if (increasing == false && firstRun == false) {
                l -= lspeed * GetFrameTime();

                if (l <= lMin) {
                    l = lMin;
                    TorL = true;
                    increasing = true;
                    tMax -= 2 * tspeed * GetFrameTime();
                }

            }

        }
    
        mu_t = static_cast<double>(t);
        mu_l = static_cast<double>(l);

        //Update point positions based on parametrisation
        point.x = static_cast<float>(parserX.Eval());;
        point.y = static_cast<float>(parserY.Eval());;
        point.z = static_cast<float>(parserZ.Eval());;

        //Update path based on point
        path.push_back(point);

        //Direct Camera
        cameraAngle += GetFrameTime() * cameraSpeed;
        float mouseWheel = GetMouseWheelMove();
        cameraRadius -= mouseWheel * cameraZoomSpeed;

        camera.position.x = cameraRadius * cos(cameraAngle);
        camera.position.y = cameraRadius * sin(cameraAngle);
        camera.position.z = cameraRadius;

        //Render
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawSphere(Vector3{point.x, point.y, point.z}, 0.01f, GRAY);

                for(size_t i = 0; i + 1 < path.size(); i++) {
                    DrawLine3D(Vector3{path[i].x, path[i].y, path[i].z}, Vector3{path[i+1].x, path[i+1].y, path[i+1].z}, RED);
                }

            EndMode3D();

        EndDrawing();

    }

    CloseWindow();
    return 0;
}