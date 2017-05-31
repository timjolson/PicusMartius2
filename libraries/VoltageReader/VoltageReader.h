#ifndef VoltageReader_h
#define VoltageReader_h

#include <Arduino.h>

#define NUM_SAMPLES 25

typedef void (*fnc_callback)(float); //Create datatype to handle function callbacks

class VoltageReader {
	public:
		VoltageReader(byte);
		VoltageReader(byte, float, float);
		VoltageReader(byte, float, float, fnc_callback, fnc_callback);
		float getVoltage();
		float getAverageVoltage();
		void check();
		boolean isLow();
		boolean isCritical();
	private:
		byte _pin;
		float _lowV;
		float _warnV;
		float _samples[NUM_SAMPLES];
		int _sampleNum;
		boolean _limitSamples;
		fnc_callback _warnCallback;
		fnc_callback _criticalCallback;
		float _getVoltage();
		float _getAverageVoltage();
};

VoltageReader::VoltageReader(byte analogPin) {
	VoltageReader(analogPin, -1, -1);
}

VoltageReader::VoltageReader(byte analogPin, float lowVoltageLevel, float warnVoltageLevel) {
	VoltageReader(analogPin, lowVoltageLevel, warnVoltageLevel, 0, 0);
}

VoltageReader::VoltageReader(byte analogPin, float lowVoltageLevel, float warnVoltageLevel, fnc_callback warningFnc, fnc_callback criticalFnc) {
	_pin = analogPin;

	_lowV = lowVoltageLevel;
	_warnV = warnVoltageLevel;

	_warnCallback = warningFnc;
	_criticalCallback = criticalFnc;

	for(int i=0; i<(sizeof(_samples)/sizeof(float)); i++) {
		_samples[i] = -1.0;
	}
	_sampleNum = 0;
	_limitSamples = true;
}

float VoltageReader::getVoltage() {
	return _getVoltage();
}

float VoltageReader::getAverageVoltage() {
	return _getAverageVoltage();
}

void VoltageReader::check() {
	if(isCritical() && _criticalCallback) {
		//_criticalCallback(getVoltage()); //Call the critical function callback
		(*_criticalCallback)(getVoltage());
	} else if(isLow() && _warnCallback) {
		//_warnCallback(getVoltage()); //Call the warning function callback
		(*_warnCallback)(getVoltage());
	}
}

boolean VoltageReader::isLow() {
	return (getVoltage() < _lowV);
}

boolean VoltageReader::isCritical() {
	return (getVoltage() < _warnV);
}

float VoltageReader::_getAverageVoltage() {
	float voltage = _getVoltage();
	_samples[_sampleNum] = voltage;
	float sum = 0.0;
	int total = 0;

	for(int i = 0; i < (sizeof(_samples)/sizeof(float)); i++) {
		if(_samples[i] > 0 || _limitSamples) {
			sum += _samples[i];
			total++;
		}
	}

	_sampleNum++;
	if(_sampleNum == NUM_SAMPLES) {
		_sampleNum = 0;
		_limitSamples = false;
	}

	return sum / total;
}

float VoltageReader::_getVoltage() {
        //12.78|934
        //12.77|936.3
	float newVoltageLevel = analogRead(_pin) / 73.320282;
//float newVoltageLevel = analogRead(_pin);
	return newVoltageLevel;
}

#endif
