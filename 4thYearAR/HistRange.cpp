#include "stdafx.h"
#include "HistRange.h"

HistRange::HistRange(int h_min, int h_max, int s_min, int s_max, int v_min, int v_max)
{
	this->h_min = h_min;
	this->h_max = h_max;
	this->s_min = s_min;
	this->s_max = s_max;
	this->v_min = v_min;
	this->v_max = v_max;
}

int HistRange::getHueMin()
{
	return h_min;
}

int HistRange::getHueMax()
{
	return h_max;
}

int HistRange::getSaturationMin()
{
	return s_min;
}

int HistRange::getSaturationMax()
{
	return s_max;
}

int HistRange::getValueMin()
{
	return v_min;
}

int HistRange::getValueMax()
{
	return v_max;
}

