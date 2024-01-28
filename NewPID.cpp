/*

 NewPID.cpp  --  An Improved PID library for Arduino
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

#include "NewPID.h"

void NewPID::enable(boolean b) {
	enabled = b;
}

void NewPID::bumplessStart(double currentInput, double currentOutput, int skips) {
	if (!enabled) {
		enabled = true;
		iTerm = currentOutput;
		lastInput = currentInput;
		if (iTerm > settings->outputMax) {
			iTerm = settings->outputMax;
		} else if (iTerm < settings->outputMin) {
			iTerm = settings->outputMin;
		}
		skipCount = skips;
		integral = 0;
	}
}


double NewPID::compute(double input) {
	
	unsigned long cm = millis();
	double deltaT = (double)(cm - lastRunTime) * 0.001;
	
	double error = settings->setpoint - input;
	
	double pTerm = settings->Kp * error;
	double dTerm = 0.0;
	
	skipCount--;
	if(skipCount <= 1){
		skipCount = 1;
		integral += error * deltaT;
		iTerm = settings->Ki * integral;
		if((iTerm > settings->outputMax) || (iTerm < settings->outputMin)) {
			integral -= error * deltaT;  // To prevent runaway windup. 
		}
		double derivative = (input = lastInput) / deltaT;
		dTerm = -(settings->Kd) * derivative;
	}
	
	double output = pTerm + iTerm + dTerm;
	
	if (output > settings->outputMax) {
		output = settings->outputMax;
	} else if (output < settings->outputMin) {
		output = settings->outputMin;
	}
	
	lastInput = input;
	lastRunTime = cm;
	
	return (settings->direction == REVERSE)? -output : output;
}


