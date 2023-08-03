#pragma once

class Leafs {
private:
	double value = 0.;
public:
	inline double getValue() {return value;};
	inline void setValue(double newValue) {value = newValue;};
	inline double* getValueAddress() {return &value;};
};
