/**
   Intelligent Surveillance System
   @file display.h
   @author Lavinan Selvaratnam
*/

#include "../frameManager/frameManager.h"

/**
    @class Class to take frames from frame queue and display them.
    @details This is a temporary class to show the intermediate output after decoding and
    storing the frames in a frame queue.
*/
class display {

    public:

        /**
            @brief Member function to run as a seperate thread and display the frames.
            @param m1 pointer to the frame queue from which frames should be taken.
        */
        void operator()(FrameManager *frameManager);
        
};