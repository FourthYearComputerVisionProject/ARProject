#ifndef HIST_RANGE_H
#define HIST_RANGE_H

class HistRange
{
public:
	HistRange(int h_min, int h_max, int s_min, int s_max, int v_min, int v_max);
	int getHueMin();
	int getHueMax();
	int getSaturationMin();
	int getSaturationMax();
	int getValueMin();
	int getValueMax();	

private:
	int h_min, h_max;
	int s_min, s_max;
	int v_min, v_max;
};
#endif