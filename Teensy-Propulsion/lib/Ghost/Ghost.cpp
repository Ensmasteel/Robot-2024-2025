#include "Ghost.h"
#include "Math_functions.h"

Ghost::Ghost(VectorOriented initialPosition) {
    currentPosition = initialPosition;
    previousPosition = initialPosition;
    delayedPosition = initialPosition;
    goaledPosition = initialPosition;
    locked = true;
    t = 0.0;
    t_delayed = 0.0;
    t_v = 0.0;
    t_v_delayed = 0.0;
    trajectory_X.setPolynome(initialPosition.getX());
    trajectory_Y.setPolynome(initialPosition.getY());
    speedSquare_v = Polynome();
    speedProfileRotation.setZeroTrapezoidalFunction();
    speedProfileLinear.setZeroTrapezoidalFunction();
}

bool Ghost::getTrajectoryIsFinished() { return trajFinished; }

void Ghost::setLock(bool lock) { locked = lock; }

bool Ghost::getLock() { return locked; }

void Ghost::goToRobot(VectorOriented currentRobotPos) { this->currentPosition = currentRobotPos; }

bool Ghost::computeTrajectory(VectorOriented endPosition, float deltaCurve, float speedRamps, float cruisingSpeed, bool isOnlyRotation, bool isBackward, bool nullInitSpeed, bool nullFinalSpeed) {
    bool boolError = false;  // booleen d'erreur

    goaledPosition = endPosition;

    t = 0.0;
    t_v = 0.0;
    t_delayed = 0.0;
    t_v_delayed = 0.0;
    pureRotation = isOnlyRotation;
    backward = isBackward;
    trajFinished = false;

    goaledPosition.normalizeTheta();
    currentPosition.normalizeTheta();

    float distanceToCover = currentPosition.distanceWith(goaledPosition);  // Norme de la distance a parcourir

    if (pureRotation) {
        lenghtTrajectory = normalizeAngle(goaledPosition.getTheta() - currentPosition.getTheta());  // Angle de rotation a effectuer

        if (abs(lenghtTrajectory) < MIN_ROTATION) {
            boolError = true;
            trajFinished = true;
        } else {
            speedProfileRotation.setTrapezoidalFunction(nullInitSpeed? speedRamps : std::numeric_limits<float>::max(), nullFinalSpeed? speedRamps : std::numeric_limits<float>::max(), cruisingSpeed, abs(lenghtTrajectory));
            durationTrajectory = speedProfileRotation.getDuration();

            trajectory_X.setPolynome(goaledPosition.getX());
            trajectory_Y.setPolynome(goaledPosition.getY());
            speedSquare_v = Polynome();

            speedProfileLinear.setZeroTrapezoidalFunction();
        }
    } else {
        if (abs(distanceToCover) < MIN_MOVEMENT) {
            boolError = true;
            trajFinished = true;
        } else {
            float x0 = currentPosition.getX();
            float y0 = currentPosition.getY();
            float x3 = goaledPosition.getX();
            float y3 = goaledPosition.getY();
            float x1;
            float y1;
            float x2;
            float y2;

            if (backward) {
                x1 = x0 + deltaCurve * distanceToCover * cos(normalizeAngle(currentPosition.getTheta() + PI));
                y1 = y0 + deltaCurve * distanceToCover * sin(normalizeAngle(currentPosition.getTheta() + PI));
                x2 = x3 - deltaCurve * distanceToCover * cos(goaledPosition.getTheta() + PI);
                y2 = y3 - deltaCurve * distanceToCover * sin(goaledPosition.getTheta() + PI);
            } else {
                x1 = x0 + deltaCurve * distanceToCover * cos(currentPosition.getTheta());
                y1 = y0 + deltaCurve * distanceToCover * sin(currentPosition.getTheta());
                x2 = x3 - deltaCurve * distanceToCover * cos(goaledPosition.getTheta());
                y2 = y3 - deltaCurve * distanceToCover * sin(goaledPosition.getTheta());
            }

            trajectory_X.setPolynome(x0, 3.0 * (x1 - x0), 3.0 * (x0 - 2 * x1 + x2), 3.0 * x1 - x0 - 3.0 * x2 + x3);
            trajectory_Y.setPolynome(y0, 3.0 * (y1 - y0), 3.0 * (y0 - 2 * y1 + y2), 3.0 * y1 - y0 - 3.0 * y2 + y3);

            Polynome speedX_v = derivativePtr(&trajectory_X);
            Polynome speedY_v = derivativePtr(&trajectory_Y);

            speedSquare_v = speedX_v * speedX_v + speedY_v * speedY_v;

            lenghtTrajectory = 0.0;

            float t_v_tmp = 0.0;           // Parcourt l'intervalle 0-1
            float deltaIntegral = 0.0005;  // Taille du mini-intervalle

            float speed_v = sqrt(speedSquare_v.f(t_v_tmp));
            float last_speed_v = sqrt(speedSquare_v.f(t_v_tmp));

            while (t_v_tmp < 1.0) {
                last_speed_v = speed_v;
                t_v_tmp += deltaIntegral;
                speed_v = sqrt(speedSquare_v.f(t_v_tmp));
                lenghtTrajectory += ((speed_v + last_speed_v) / 2.0) * deltaIntegral;
            }

            speedProfileLinear.setTrapezoidalFunction(nullInitSpeed? speedRamps : std::numeric_limits<float>::max(), nullFinalSpeed? speedRamps : std::numeric_limits<float>::max(), cruisingSpeed, abs(lenghtTrajectory));
            durationTrajectory = speedProfileRotation.getDuration();
        }
    }

    return boolError;
}

void Ghost::actuatePosition(float dt) {
    t += dt;
    t_delayed = ((t > DELAY_POSITION) ? t - DELAY_POSITION : 0.0);

    if (!locked) {
        if ((t_v_delayed <= 1.0)) {
            if (pureRotation) {
                previousPosition = currentPosition;
                float curr_th = currentPosition.getTheta();
                float del_th = delayedPosition.getTheta();
                currentPosition.setTheta(curr_th + speedProfileRotation.f(t) * dt * copysignf(1.0, lenghtTrajectory));
                currentPosition.normalizeTheta();
                delayedPosition.setTheta(del_th + speedProfileRotation.f(t_delayed) * dt * copysignf(1.0, lenghtTrajectory));
                delayedPosition.normalizeTheta();

                t_v = t / durationTrajectory;
                t_v_delayed = t_delayed / durationTrajectory;
            }

            else {
                float speed_v = sqrt(speedSquare_v.f(t_v));  // Vitesse virtuelle associee au courbes de Bezier
                float speed = speedProfileLinear.f(t);       // Vitesse reelle voulue

                if (speed_v != 0.0) {
                    t_v += (speed / speed_v) * dt;
                }

                t_v = min(1.0, t_v);

                float speed_v_delayed = sqrt(speedSquare_v.f(t_v_delayed));  // Vitesse virtuelle reportee associee au courbes de Bezier
                float speed_delayed = speedProfileLinear.f(t_delayed);       // Vitesse reelle reportee voulue

                if (speed_v_delayed != 0.0) {
                    t_v_delayed += (speed_delayed / speed_v_delayed) * dt;
                }

                previousPosition = currentPosition;

                delayedPosition.setX(trajectory_X.f(t_v_delayed));
                delayedPosition.setY(trajectory_Y.f(t_v_delayed));
                currentPosition.setX(trajectory_X.f(t_v));
                currentPosition.setY(trajectory_Y.f(t_v));

                if (abs(speed_v) > 1e-9) {
                    if (backward) {
                        delayedPosition.setTheta(atan2(-trajectory_Y.df(t_v_delayed), -trajectory_X.df(t_v_delayed)));
                        currentPosition.setTheta(atan2(-trajectory_Y.df(t_v), -trajectory_X.df(t_v)));
                    } else {
                        delayedPosition.setTheta(atan2(trajectory_Y.df(t_v_delayed), trajectory_X.df(t_v_delayed)));
                        currentPosition.setTheta(atan2(trajectory_Y.df(t_v), trajectory_X.df(t_v)));
                    }
                }

                else {
                    delayedPosition.setTheta(currentPosition.getTheta());
                }
            }
        }
    }

    updateSpeeds(currentPosition, previousPosition, dt);

    if (t_v_delayed > 0.999) {
        if (delayedPosition.distanceWith(goaledPosition) < MAX_DISTANCE)  // If we're not to far from the goal
        {
            if (pureRotation) {
                currentPosition.setTheta(goaledPosition.getTheta());
                delayedPosition.setTheta(goaledPosition.getTheta());
                previousPosition.setTheta(goaledPosition.getTheta());
            } else {
                currentPosition = goaledPosition;
                delayedPosition = goaledPosition;
                previousPosition = goaledPosition;
            }

            trajFinished = true;
        }
    }

    //currentPosition.printTeleplot("GHOST ");
}

void Ghost::updateSpeeds(VectorOriented curPosition, VectorOriented lastPosition, float dt) {
    currentSpeedLinear = (pureRotation ? 0.0 : 1.0) * (backward ? -1.0 : 1.0) * curPosition.distanceWith(lastPosition) / dt;
    currentSpeedRotation = normalizeAngle(curPosition.getTheta() - lastPosition.getTheta()) / dt;
}

Kinetic Ghost::getControllerKinetic()  // On fait appel a cete fonction pas mal de fois, est ce qu'on créé par pleins d'objets Kinetic ??
{
    Kinetic out;

    out.setX(delayedPosition.getX());
    out.setY(delayedPosition.getY());
    out.setTheta(delayedPosition.getTheta());
    out.setTranslationSpeed(currentSpeedLinear);
    out.setRotationSpeed(currentSpeedRotation);

    return out;
}
