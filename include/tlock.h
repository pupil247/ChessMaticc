/**
 * @file tlock.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief mutex handler for real time applications made with esp32
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "mutex.h"

/**
 * @brief tlock class
 * 
 */
class TLock {
private:
    Mutex* mutex;   
public:
    /**
     * @brief Construct a new TLock object, take the mutex on create
     * 
     * @param mutex 
     */
    TLock(Mutex* mutex){
        
        this->mutex = mutex;
        mutex->take();
    };
    /**
     * @brief Destroy the TLock object, release the mutex on destroy
     * 
     */
    ~TLock(){
        if(mutex)
            mutex->release();
    }

};