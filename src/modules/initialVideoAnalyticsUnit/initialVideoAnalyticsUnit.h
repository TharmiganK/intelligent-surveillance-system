/**
    Intelligent Surveillance System
    @file initialVideoAnalyticsUnit.h
    @author Tharmigan Krishnanathalingam
*/

#ifndef _INITIALVIDEOANALYTICSUNIT_H
#define _INITIALVIDEOANALYTICSUNIT_H

#include "../backgroundExtractor/backgroundExtractor.h"
#include "../opticalFlowEstimator/opticalFlowEstimator.h"
#include <thread>
#include <iostream>

class InitialVideoAnalyticsUnit {

    public :

        InitialVideoAnalyticsUnit(){};

        void operator()(VideoStream& videoStream);

};

#endif