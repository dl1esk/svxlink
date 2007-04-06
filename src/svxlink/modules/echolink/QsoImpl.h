/**
@file	 QsoImpl.h
@brief   Data for one EchoLink Qso.
@author  Tobias Blomberg / SM0SVX
@date	 2004-06-02

This file contains a class that implementes the things needed for one
EchoLink Qso.

\verbatim
<A brief description of the program or library this file belongs to>
Copyright (C) 2004  Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

/** @example Template_demo.cpp
An example of how to use the Template class
*/


#ifndef QSO_IMPL_INCLUDED
#define QSO_IMPL_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <string>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <EchoLinkQso.h>
#include <EchoLinkStationData.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/

namespace Async
{
  class Config;
};


/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

//namespace MyNameSpace
//{


/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class MsgHandler;
class EventHandler;
class AudioPacer;
class AsyncTimer;
class ModuleEchoLink;


/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	Implements the things needed for one EchoLink QSO.
@author Tobias Blomberg
@date   2004-06-02

A class that implementes the things needed for one EchoLink Qso.
*/
class QsoImpl : public EchoLink::Qso
{
  public:
    /**
     * @brief 	Default constuctor
     */
    QsoImpl(const EchoLink::StationData &station, ModuleEchoLink *module);
  
    /**
     * @brief 	Destructor
     */
    virtual ~QsoImpl(void);
  
    /**
     * @brief 	Check that the initialization went ok
     * @return	Returns \em true if the initialization was ok or \em false on
     *	      	failure
     *
     * This function should be called after creating a new Qso object to make
     * sure everything went well.
     */
    bool initOk(void);

    /**
     * @brief 	Send audio to the remote station
     * @param 	buf A buffer containing 16 bit samples to send
     * @param 	len The length, in samples, of the buffer to send
     * @return	Returns the number of samples written
     *
     * This function is used to send audio to the remote station. This
     * reimplemented version of EchoLink::Qso::sendAudio throws away
     * the samples if a message is being played to the remote station.
     */
    int sendAudio(float *buf, int len);
    
    /**
     * @brief 	Send a raw GSM audio packet to the remote station
     * @param 	packet The packet to send
     *
     * This function can be used to send a raw GSM packet to the remote
     * station. Probably only useful if you received it from the
     * audioReceivedRaw signal.
     */
    bool sendAudioRaw(GsmVoicePacket *packet);
    
    /**
     * @brief 	Initiate a connection to the remote station
     * @return	Returns \em true if the connect message was sent ok or \em false
     *        	on failure
     *
     * Use this function to connect to the remote station. The \em StateChange
     * signal will be emitted to indicate that a connection is in progress.
     * When the connection has been established, the \em stateChange signal
     * will be emitted again. On failure to connect, the \em stateChange
     * signal will be emitted to indicate that the disconnected state
     * has been entered again.
     */
    bool connect(void);
    
    /**
     * @brief 	Accept an incoming connection
     * @return	Returns \em true if the connect message was sent successfully or
     *	      	\em false on failure
     *
     * Use this function to accept an incoming connection. Incoming connections
     * are signalled through the EchoLink::Dispatcher. When an incoming
     * connection has been received, a Qso object should be created and this
     * function should be called to accept the connection. Be sure to check that
     * a valid callsign has connected. At least if the EchoLink node is
     * connected to a radio transmitter.
     *
     * The difference between the connect and accept functions are that the
     * accept function goes right into the connected state. The remote station
     * is assumed to be present. This might not be true in some strange cases.
     * In such a strange case, the connection will timeout after a while.
     */
    bool accept(void);
    
    /**
     * @brief 	Reject the connection
     * @param 	perm If \em true the rejection is permanent. If \em false
     *               the rejection is temporary.
     */
    void reject(bool perm);
    
    /**
     * @brief 	Return the StationData object associated with this QSO
     * @return	Returns a reference to the associated StationData object
     */
    const EchoLink::StationData& stationData(void) const { return station; }


    /**
     * @brief A signal that is emitted when the connection state changes
     * @param qso The QSO object
     * @param state The new connection state
     */
    SigC::Signal2<void, QsoImpl*, Qso::State> stateChange;
    
    /**
     * @brief A signal that is emitted when a station info message is received
     * @param qso The QSO object
     * @param msg The received message
     */
    //SigC::Signal2<void, QsoImpl*, const std::string&> infoMsgReceived;
    
    /**
     * @brief A signal that is emitted when a chat message is received
     * @param qso The QSO object
     * @param msg The received chat message
     */
    SigC::Signal2<void, QsoImpl*, const std::string&> chatMsgReceived;
    
    /**
     * @brief A signal that is emitted when the audio receive state changes
     * @param is_receiving  Is \em true when audio is being received and
     *                      \em false when not
     * @param qso The QSO object
     * @note This signal can be used to control a reception indicator
     */
    SigC::Signal2<void, bool, QsoImpl*> isReceiving;
    
    /**
     * @brief A signal that is emitted when an audio datagram has been received
     * @param buf A pointer to the buffer that contains the audio
     * @param qso The QSO object
     * @param len The number of samples in the buffer
     */
    SigC::Signal3<int, float*, int, QsoImpl*> audioReceived;
    
    /**
     * @brief A signal that is emitted when an audio datagram has been received
     * @param packet A pointer to the buffer that contains the raw GSM audio
     * @param qso The QSO object
     */
    SigC::Signal2<void, Qso::GsmVoicePacket*, QsoImpl*> audioReceivedRaw;
    
    /**
     * @brief 	A signal that is emitted when the qso object should be destroyed
     * @param 	qso The QSO object
     */
    SigC::Signal1<void, QsoImpl*> destroyMe;
    
    
    /**
     * @brief 	A_brief_member_function_description
     * @param 	param1 Description_of_param1
     * @return	Return_value_of_this_member_function
     */

    
  protected:
    
  private:
    ModuleEchoLink    	  *module;
    EventHandler      	  *event_handler;
    MsgHandler	      	  *msg_handler;
    AudioPacer       	  *msg_pacer;
    bool      	      	  init_ok;
    bool      	      	  reject_qso;
    std::string       	  last_message;
    std::string       	  last_info_msg;
    Async::Timer      	  *idle_timer;
    bool      	      	  disc_when_done;
    int       	      	  idle_timer_cnt;
    int       	      	  idle_timeout;
    Async::Timer	  *destroy_timer;
    EchoLink::StationData station;
    
    void allRemoteMsgsWritten(void);
    void onInfoMsgReceived(const std::string& msg);
    void onChatMsgReceived(const std::string& msg);
    void onStateChange(Qso::State state);
    void idleTimeoutCheck(Async::Timer *t);
    void destroyMeNow(Async::Timer *t);

};  /* class QsoImpl */


//} /* namespace */

#endif /* QSO_IMPL_INCLUDED */



/*
 * This file has not been truncated
 */

