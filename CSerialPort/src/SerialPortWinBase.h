#ifndef __CSERIALPORTWINBASE_H__
#define __CSERIALPORTWINBASE_H__

#include "SerialPortBase.h"

// windows need
#include <windows.h>
#include <tchar.h>//tchar
#include <atlstr.h>//_tcscpy_s

// sigslot
#include "sigslot.h"

/***************************************

* 1. ���߳��ڲ��ӹܣ����迪���߹���
* 2. ʹ���Զ���������̻߳����������
* 3. ���Ϊ�麯���������߿��Լ̳��޸�
* 4. ֧���Ȳ��
* 5. ֧��ͨ��ͨ��Э��:֡ͷ + ���� + У�� + ֡β
* 6. ֧�ֻ���ʱ��Ƭ��������װ
* 7. ����ʱ��������ݻ������ - ���ݴ浽�������������ʱ��� �����߰���һ�����Ի�ȡ����װ����
* 8. ��дwindows��ReadFile��WriteFile

*/

class CSerialPortWinBase :	public CSerialPortBase
{
public:
	CSerialPortWinBase();
	CSerialPortWinBase(const std::string & portName);
	virtual ~CSerialPortWinBase();

	virtual void construct();

	virtual void init(std::string portName, int baudRate = itas109::BaudRate9600, itas109::Parity parity = itas109::ParityNone, itas109::DataBits dataBits = itas109::DataBits8, itas109::StopBits stopbits = itas109::StopOne, itas109::FlowConctrol flowConctrol = itas109::FlowNone, int64 readBufferSize = 512);
	virtual void init(int port, int baudRate = itas109::BaudRate9600, itas109::Parity parity = itas109::ParityNone, itas109::DataBits dataBits = itas109::DataBits8, itas109::StopBits stopbits = itas109::StopOne, itas109::FlowConctrol flowConctrol = itas109::FlowNone, int64 readBufferSize = 512);

	virtual bool open();
	virtual void close();

	virtual bool isOpened();

	virtual int readData(char *data, int maxSize);
	virtual int readAllData(char *data);
	virtual int	readLineData(char *data, int maxSize);
	virtual int writeData(const char * data, int maxSize);

	virtual void setDebugModel(bool isDebug);
	 
	virtual void setReadTimeInterval(int msecs);

	virtual int getLastError() const;
	virtual void clearError();

	virtual void setPortName(std::string portName);
	virtual std::string getPortName() const;

	virtual void setBaudRate(int baudRate);
	virtual int getBaudRate() const;

	virtual void setParity(itas109::Parity parity);
	virtual itas109::Parity getParity() const;

	virtual void setDataBits(itas109::DataBits dataBits);
	virtual itas109::DataBits getDataBits() const;

	virtual void setStopBits(itas109::StopBits stopbits);
	virtual itas109::StopBits getStopBits() const;

	virtual void setFlowConctrol(itas109::FlowConctrol flowConctrol);
	virtual itas109::FlowConctrol getFlowConctrol() const;

	virtual void setReadBufferSize(int64 size);
	virtual int64 getReadBufferSize() const;

	virtual void setDtr(bool set = true);
	virtual void setRts(bool set = true);

	std::string getVersion();

public:
	OVERLAPPED getOverlapMonitor();
	HANDLE getMainHandle();

protected:
	void lock();
	void unlock();

private:
	static unsigned int __stdcall commThreadMonitor(LPVOID pParam);//thread monitor
	bool startThreadMonitor();
	bool stopThreadMonitor();

public:
	static sigslot::signal0<> readReady;//sigslot

private:
	std::string m_portName;
	int m_baudRate;
	itas109::Parity m_parity;
	itas109::DataBits m_dataBits;
	itas109::StopBits m_stopbits;
	enum itas109::FlowConctrol m_flowConctrol;
	int64 m_readBufferSize;

private:
	//HANDLE m_thread;//
	HANDLE m_handle;

	HANDLE m_monitorThread;
	OVERLAPPED overlapMonitor;	//monitor overlapped
	
	OVERLAPPED m_overlapRead;	//read overlapped
	OVERLAPPED m_overlapWrite;	//write overlapped

	COMMCONFIG m_comConfigure;
	COMMTIMEOUTS m_comTimeout;

	CRITICAL_SECTION m_communicationMutex;///mutex

	static bool isThreadRunning;
};
#endif //__CSERIALPORTWINBASE_H__
