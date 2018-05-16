#include <iostream>
#include "brick.h"
using namespace std;

/*
 Accurate (double-precision) Stream Check

 In a nutshell, not even a bit is missed!

 Stream function is validated with double precision accuracy. The stream function is periodic i.e.
 if any thing goes out of the brick from right, it comes into brick from left.
 A small brick is selected, like 5x5. The distribution function of all nodes are set to 0. Only at
 one node, like brick(3,3), the distrobution function at a specific direction, like f[1], is set to
 1.0. In each streaming time step we track the value of 1.0. A correct stream in the system should
 show a horizental movement of that value. It is repeated for all the other directions too.
 Note if direction is zero, the package doesn't move.
 */

int trackF(const Brick& brick, double& epsilon) {
    int found = 0;
    for (int iX = 0; iX < brick.getNX(); ++iX) {
        for (int iY = 0; iY < brick.getNY(); ++iY) {
            for (int iQ = 0; iQ < lattice::nQ; ++iQ) {
                if (brick(iX, iY)[iQ] > epsilon) {
                    ++found;
                    cout << "The package is now at iX=" << iX << ", iY=" << iY
                            << ", Q direction=" << iQ << " with value of ="
                            << brick(iX, iY)[iQ] << endl;
                }
            }
        }
    }
    return found;
}

int main() {

    // Starting Point, choose for example center, corner (0,0), and etc.
    int iX = 0;
    int iY = 0;
    // Brick size
    int nX = 5;
    int nY = 5;

    Brick brick(nX, nY);
    double epsilon = (double) 0.000000000000001;
    double package_value = (double) 1.000000000000001;
    cout.precision(16);
    int found;

    for (int iQ = 0; iQ < lattice::nQ; ++iQ) {
        cout << "=========  A new direction is tested: iQ= " << iQ
                << "===========" << endl;
        brick(iX, iY)[iQ] = package_value;
        for (int t = 0; t < 10; ++t) {
            brick.stream();
            cout << "----- TimeStep = " << t << " -----" << endl;
            found = trackF(brick, epsilon);
            if (found != 1) {
                cout << "Something happend to the package!";
                break;
            }
        }
        brick = (double) 0.;
    }
    return 0;
}

