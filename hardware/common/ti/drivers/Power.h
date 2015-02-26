/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       Power.h
 *
 *  @brief      Power manager interface
 *
 *  The Power header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/Power.h>
 *  @endcode
 *
 *  # Operation #
 *  The Power manager facilitates the transition of the MPU from active state
 *  to one of the sleep states and vice versa.  It provides drivers the
 *  ability to set and release dependencies on hardware resources and keeps
 *  a reference count on each resource to know when to enable or disable the
 *  peripheral clock to the resource.  It provides drivers the ability to
 *  register a callback function upon a specific power event.  In addition,
 *  drivers and apps can set or release constraints to prevent the MCU from
 *  transitioning into a particular sleep state.
 *
 *  # Calling Context #
 *
 *  <table>
 *    <tr>
 *      <th> Function </th><th> Hwi </th><th> Swi </th><th> Task </th>
 *      <th> Main </th>
 *    </tr>
 *    <tr>
 *      <td> Power_getConstraintMask </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_getDependencyCount </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_getPerformanceLevel </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_getTransitionLatency </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_getTransitionState </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_idleFunc </td>
 *      <td> N </td><td> N </td><td> N </td><td> N </td>
 *    </tr>
 *    <tr>
 *      <td> Power_init </td>
 *      <td> N </td><td> N </td><td> N </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_registerNotify </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_releaseConstraint </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_releaseDependency </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_setConstraint </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_setDependency </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_setPerformanceLevel </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_shutdown </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td> Power_sleep </td>
 *      <td> N </td><td> N </td><td> N </td><td> N </td>
 *    </tr>
 *    <tr>
 *      <td> Power_unregisterNotify </td>
 *      <td> Y </td><td> Y </td><td> Y </td><td> Y </td>
 *    </tr>
 *    <tr>
 *      <td colspan="5">
 *        <ul>
 *          <li><b>Hwi</b>: API is callable from Hwi context. </li>
 *          <li><b>Swi</b>: API is callable from Swi context. </li>
 *          <li><b>Task</b>: API is callable from a Task thread. </li>
 *          <li><b>Main</b>: API is callable from main(). </li>
 *        </ul>
 *      </td>
 *    </tr>
 *  </table>
 *
 *  ============================================================================
 */

#ifndef ti_drivers_Power__include
#define ti_drivers_Power__include

#include <stdint.h>
#include <ti/drivers/ports/ListP.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Power latency types */
/*! total latency type */
#define Power_TOTAL                 1
/*! resume latency type */
#define Power_RESUME                2

/* Power notify responses */
/*! notify done value */
#define Power_NOTIFYDONE            0
/*! notify error value */
#define Power_NOTIFYERROR           1

/* Power status */
/*! ok status */
#define Power_SOK                   0
/*! ok status with the state restored */
#define Power_SOK_STATE_RESTORED    1
/*! error failed status */
#define Power_EFAIL                 2
/*! error invalid pointer status */
#define Power_EINVALIDPOINTER       3
/*! error change not allowed status */
#define Power_ECHANGE_NOT_ALLOWED   4
/*! error busy status */
#define Power_EBUSY                 5

/* Power transition states */
/*! active state */
#define Power_ACTIVE                1
/*! entering sleep state */
#define Power_ENTERING_SLEEP        2
/*! exiting sleep state */
#define Power_EXITING_SLEEP         3
/*! entering shutdown state */
#define Power_ENTERING_SHUTDOWN     4

/*!
 *  @brief      Power policy init function pointer
 */
typedef void (*Power_PolicyInitFxn)(void);

/*!
 *  @brief      Power policy function pointer
 */
typedef void (*Power_PolicyFxn)(void);

/*!
 *  @brief      Power notify function pointer
 */
typedef int (*Power_NotifyFxn)(uint32_t eventType, uint32_t eventArg,
                               uint32_t clientArg);

/*!
 *  @brief      Power notify object structure.
 *
 *  The 'eventTypes' are device specific.  For example on the CC3200
 *  'PowerCC3200_ENTERING_DEEPSLEEP' is an event type.
 */
typedef struct Power_NotifyObj {
    ListP_Elem link;
    uint32_t eventTypes;
    Power_NotifyFxn notifyFxn;
    uint32_t clientArg;
} Power_NotifyObj;

/*!
 *  @brief  Function tells which constraints have been registered
 *          with Power module.
 *
 *  @return A bitmask of registered constraints
 *
 *  @sa     Power_getContraint
 */
uint32_t Power_getConstraintMask(void);

/*!
 *  @brief  Function gets the count of dependencies that are currently
 *          declared upon a resource.
 *
 *  @param  resourceId  resource id
 *
 *  @return dependency count on resource
 *
 *  @sa     Power_getDependency
 */
uint32_t Power_getDependencyCount(uint8_t resourceId);

/*!
 *  @brief  Get the performance level
 */
void Power_getPerformanceLevel(void);

/*!
 *  @brief  Function gets the transition latency for a sleep state.
 *          The latency is reported in units of microseconds
 *
 *  @param  sleepState  the power sleep state
 *
 *  @param  type        the type {Power_TOTAL, Power_RESUME}
 *
 *  @return the latency value
 */
uint32_t Power_getTransitionLatency(uint32_t sleepState, uint32_t type);

/*!
 *  @brief  Function gets the current power transition state
 *
 *  @return the current power transition state
 */
uint32_t Power_getTransitionState(void);

/*!
 *  @brief  Add this function to the system's idle loop. It calls the
 *          configured 'runPolicy' in the user defined Power config object.
 */
void Power_idleFunc(void);

/*!
 *  @brief  Policy init function
 *
 *  This function must be called prior to any power API being called.
 *  It calls the configured 'PolicyInitFxn' in the user defined
 *  config object.  The policy init function may create a clock
 *  object.
 */
void Power_init(void);

/*!
 *  @brief  Registers a function to be called on a specific power event
 *
 *  @param  pNotifyObj              notification object
 *
 *  @param  eventTypes              event type
 *
 *  @param  notifyFxn               function to be called
 *
 *  @param  clientArg               client argument
 *
 *  @return status
 *
 *  @sa     Power_unregisterNotify
 */
uint32_t Power_registerNotify(Power_NotifyObj *pNotifyObj,
    uint32_t eventTypes, Power_NotifyFxn notifyFxn, uint32_t clientArg);

/*!
 *  @brief  Releases a previously declared constraint
 *
 *  @param  constraintId      constraint id specified
 *
 *  @sa     Power_setContraint
 */
void Power_releaseConstraint(uint32_t constraintId);

/*!
 *  @brief  Releases a previously declared dependency
 *
 *  @param  resourceId      resource id specified
 *
 *  @sa     Power_setDependency
 */
void Power_releaseDependency(uint8_t resourceId);

/*!
 *  @brief  Declare an operational constraint.
 *
 *  Each call to this function must set only one constrait.
 *  To set multiple constraints, this function can be called multiple times.
 *
 *  @param  constraintId      constraint id specified
 *
 *  @sa     Power_releaseContraint
 */
void Power_setConstraint(uint32_t constraintId);

/*!
 *  @brief  Declare a dependency on a resource
 *
 *  @param  resourceId      resource id specified
 *
 *  @sa     Power_releaseDependency
 */
void Power_setDependency(uint8_t resourceId);

/*!
 *  @brief  Sets the performance level
 */
void Power_setPerformanceLevel(void);

/*!
 *  @brief  Put the device in a shutdown state.
 *
 *  @param  shutdownState  the shutdown state
 *
 *  @param  arg0           argument
 *
 *  @return status
 */
uint32_t Power_shutdown(uint32_t shutdownState, uint32_t arg0);

/*!
 *  @brief  Called from the policy function to enter sleep mode.
 *
 *  This function is called from the power policy function. It must be
 *  called with interrupts disabled and should not be called by the
 *  application or any drivers.
 *
 *  @param  sleepState      cpu is to enter this sleep state
 *
 *  @param  arg0            first argument
 *
 *  @param  arg1            second argument
 *
 *  @return status
 */
uint32_t Power_sleep(uint32_t sleepState, uint32_t arg0, uint32_t arg1);

/*!
 *  @brief  Unregister a previously registered notification
 *
 *  @param  pNotifyObj      notification object
 */
void Power_unregisterNotify(Power_NotifyObj *pNotifyObj);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_Power__include */