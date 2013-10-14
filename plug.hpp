/*
 * Simulation@Home Competition
 * File: plug.hpp
 * Author: Jiongkun Xie
 * Affiliation: Multi-Agent Systems Lab.
 *              University of Science and Technology of China
 */

#ifndef __home_plug_HPP__
#define __home_plug_HPP__

#include <string>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

namespace _home
{
    class Message;

    /** 
     * @class Plug
     * defines a common entry for platform of Home Simulation.
     */
    class Plug
    {
    private:
        class TKernel;
        typedef boost::shared_ptr<Message> MessagePtr;

    public:
        /** 
         * Destructor
         */
        virtual ~Plug();

        /** 
         * Run the client's event processing loop for the test.
         */
        void Run();

    protected:
        /** 
         * Constructor
         * @param _name is the team name
         */
        Plug(const std::string& _name);

        /**
         * The processing of a plan should be implemented in this function.
         */
        virtual void Plan() = 0;

        /**
         * If something need to be destructed after planning,
         * this function should be overloaded.
         */
        virtual void Fini() {}

        /**
         * Gets the team name.
         */
        const std::string& GetName() const { return mName; }

        /**
         * Gets the name of the test
         */
        const std::string& GetTestName() const { return mTestName; }

        /**
         * Gets the description of the environment.
         */
        const std::string& GetEnvDes() const { return mEnvDes; }

        /**
         * Gets the description of the task.
         */
        const std::string& GetTaskDes() const { return mTaskDes; }

        /** 
         * Atomic action Move
         * @param x location number
         * @return if the action is successful or not
         */
        bool Move(unsigned int x);

        /** 
         * Atomic action PickUp
         * @param a object number
         * @return if the action is successful or not
         */
        bool PickUp(unsigned int a);

        /** 
         * Atomic action PutDown
         * @param a object number
         * @return if the action is successful or not
         */
        bool PutDown(unsigned int a);

        /** 
         * Atomic action ToPlate
         * @param a object number
         * @return if the action is successful or not
         */
        bool ToPlate(unsigned int a);

        /** 
         * Atomic action FromPlate
         * @param a object number
         * @return if the action is successful or not
         */
        bool FromPlate(unsigned int a);

        /** 
         * Atomic action Open
         * @param a object number
         * @return if the action is successful or not
         */
        bool Open(unsigned int a);

        /** 
         * Atomic action Close
         * @param a object number
         * @return if the action is successful or not
         */
        bool Close(unsigned int a);

        /** 
         * Atomic action PutIn
         * @param a small object number
         * @param b big object number
         * @return if the action is successful or not
         */
        bool PutIn(unsigned int a, unsigned int b);

        /** 
         * Atomic action TakeOut
         * @param a small object number
         * @param b big object number
         * @return if the action is successful or not
         */
        bool TakeOut(unsigned int a, unsigned int b);

        /** 
         * Atomic action AskLoc
         * @param a object number
         * @return "not_known" means don't know where 'a' is
         *  "(on a b)", "(near a b)", or "(inside a b)" means
         *  the spacial relation with 'b'
         */
        std::string AskLoc(unsigned int a);

        /**
         * Atomic action Sense
         * @param A_ return set of numbers for the object observed
         */
        void Sense(std::vector<unsigned int>& A_);

    private:
        /**
         * Connects to the server.
         */
        bool Conncet();

        /** 
         * Communicate with the server.
         */
        MessagePtr SendMsg(const MessagePtr& msg);

    private:
        std::string mName;

        std::string mTestName;

        std::string mEnvDes;

        std::string mTaskDes;

        TKernel* mKernel;

        boost::array<boost::uint8_t, 4> mMsgLen;
    };//Plug

}//_home

#endif//__home_plug_HPP__
//end of file
