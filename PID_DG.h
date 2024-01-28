/*

 PID_DG.h  --  An Improved PID library for Arduino
 Copyright (C) 2024  David C.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef NEWPID_H
#define NEWPID_H

#include "Arduino.h"

enum PID_Direction {
	DIRECT, REVERSE
};

struct PID_Settings {
	double setpoint;

	double Kp;
	double Ki;
	double Kd;

	double outputMax;
	double outputMin;

	PID_Direction direction;

};

class PID_Class {
private:
	PID_Settings &settings;
	
	unsigned long lastRunTime;
	double iTerm;
	double integral;
	double lastInput;
	int skipCount;
	

public:

	PID_Class(PID_Settings &aSettings) :
			settings(aSettings) {
	}

	void bumplessStart(double currentInput, double currentOutput, int skips);
	double compute(double input);

};






#endif // NEWPID_H



