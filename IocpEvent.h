#pragma once

class Session;
enum class EventType : uint8
{
    Connect,
    DisConnect,
    Accept,
    Recv,
    Send
};

/*------------------
     IocpEvent
--------------------*/
class IocpEvent : public OVERLAPPED
{
public:
    IocpEvent(EventType type);

    void Init();
public:
    EventType eventType;
    IocpObjectRef owner;
};

/*------------------
    ConnectEvent
--------------------*/
class ConnectEvent : public IocpEvent
{
public:
    ConnectEvent() :IocpEvent(EventType::Connect) {}
};

/*------------------
    DisConnectEvent
--------------------*/
class DisConnectEvent : public IocpEvent
{
public:
    DisConnectEvent() :IocpEvent(EventType::DisConnect) {}
};

/*------------------
    AcceptEvent
--------------------*/
class AcceptEvent : public IocpEvent
{
public:
    AcceptEvent() :IocpEvent(EventType::Accept) {}

public:
    //TODO accept의 경우 인자가 추가적으로 있을수있다.
    SessionRef session = nullptr;
};

/*------------------
    RecvEvent
--------------------*/
class RecvEvent : public IocpEvent
{
public:
    RecvEvent() :IocpEvent(EventType::Recv) {}
};


/*------------------
    SendEvent
--------------------*/
class SendEvent : public IocpEvent
{
public:
    SendEvent() :IocpEvent(EventType::Send) {}

    //TEMP
    Vector<SendBufferRef> sendbuffers;
};