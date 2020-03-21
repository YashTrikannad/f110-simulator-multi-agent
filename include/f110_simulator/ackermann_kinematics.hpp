#pragma once

#include "f110_simulator/Pose2d.h"

namespace racecar_simulator {

class AckermannKinematics {

public:

    static double angular_velocity(
            double velocity,
            double steering_angle,
            double wheelbase);

    static f110_simulator::Pose2d update(
            const f110_simulator::Pose2d start,
            double velocity,
            double steering_angle,
            double wheelbase,
            double dt);

};

}
