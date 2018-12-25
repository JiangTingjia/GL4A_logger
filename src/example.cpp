
#include "../include/GL4A_user.h"

using namespace GL4A;

// �����÷�
void baseUsage()
{
	cout << "��ӭʹ��GL4A�������÷�" << endl;
	cout << "�汾��" << GL4A_VERSION << endl;
	errHandle err = errHandle();
	// ��ʼ����־�⣨�ڳ�����ڴ���
	cout << "��GL4A���г�ʼ��......��Ĭ�������ļ�·����" << GL4A_DEFAULT_CONFIG_DIR << endl;
	err = GL4A_init(GL4A_DEFAULT_CONFIG_DIR);
	if (err == 0)
	{
		cout << "��־���ʼ���ɹ�" << endl;
	}
	else
	{
		cout << "��־���ʼ��ʧ��" << endl;
	}

	// �� ��� M0 ����� TRACE �� ERROR
	// ����һ��Ŀ¼������log�ļ��У��ֱ���critical/rolling/daily�����ļ���
	// ����Ĭ�������ļ���ֻ������daily��־��¼�������н�daily�м�¼�� trace �� error
	GL4A_TRACE_LOG("M0", __F__, __L__, "TRACE ���Գɹ���\n");
	GL4A_ERROR_LOG("M0", __F__, __L__, "ERROR ���Գɹ���\n");

	// .... �������

	// �ر���־�⣨�ڳ����˳�ʱ��
	cout << "��־��ʹ�����!" << endl;
	GL4A_close();
	genConfigDefault();
}

// ���������ļ�
void changeConfig()
{
	
	cout << "��ӭʹ��GL4A�����������ļ�" << endl;
	cout << "�汾��" << GL4A_VERSION << endl;
	errHandle err = errHandle();
	cout << "��GL4A���г�ʼ��......��Ĭ�������ļ�·����" << GL4A_DEFAULT_CONFIG_DIR << endl;
	err = GL4A_init(GL4A_DEFAULT_CONFIG_DIR);
	if (err == 0)
	{
		cout << "��־���ʼ���ɹ�" << endl;
	}
	else
	{
		cout << "��־���ʼ��ʧ��" << endl;
	}

	// ���M0��� ��Ϣ
	GL4A_TRACE_LOG("M0", __F__, __L__, "�Һ���ɹ��ˣ�\n");

	// ���M1�����Ϣ
	GL4A_TRACE_LOG("M1", __F__, __L__, "�Һ���ʧ���ˣ�ΪɶM0�ܳɹ���M1ȴ���ܳɹ���M1Ҳ��ɹ���\n");

	// ��ȡ�����ļ�����
	TotalCfgInfos cfg = TotalCfgInfos();
	err = loadCfgFileToStruct(GL4A_DEFAULT_CONFIG_DIR, cfg);

	// ���Բ����㣬Ҫ��Ҳ��console ����̨Ҳ���һ�°�
	bool consoleState = cfg.getConsoleState();
	cout << string("console state:") << ((true == consoleState) ? string("true") : string("false")) << endl;

	cfg.setConsoleState(true);

	// �鿴�����ļ������
	vector<pair<string, bool>> models = cfg.catchModels();
	for (size_t i = 0; i < models.size(); i++)
	{
		cout << "�����:" << models.at(i).first << "״̬:" << models.at(i).second << endl;
	}
	cout << "ԭ��M1������������ļ��У�" << endl;

	// �Ҿ������M1,�һ������DOG 
	cfg.addModel("M1", true);
	cfg.addModel("DOG", true);

	// ����һ�������ļ��������¼���һ��
	saveCfgFileFromStruct(GL4A_DEFAULT_CONFIG_DIR, cfg);
	GL4A_reloadConfig(GL4A_DEFAULT_CONFIG_DIR);

	// ��һ��,��console�л��������DOG��Ϣ���������� ERROR�ȼ���TRACE�ߣ��ڸߵȼ����������Ϣһ�����ڵ͵ȼ����������������TRACE�лᱣ֤���ݵ�������������
	GL4A_TRACE_LOG("M1", __F__, __L__, "�Һ���ɹ���\n");
	GL4A_ERROR_LOG("DOG", __F__, __L__, "�Һ���ɹ���\n");

	// �㻹����
	// ����ĳ������Ƿ�����
	cfg.changeModelState("M0", false);
	// ����������
	cfg.clearModels();
	// ɾ��ĳ�����
	cfg.deleteModel("M0");
	// ��ȡconsole ����״̬
	bool state = cfg.getConsoleState();
	// ...����μ� TotalCfgInfos �� 


	// �ر���־�⣨�ڳ����˳�ʱ��
	cout << "��־��ʹ�����!" << endl;
	GL4A_close();
}

// ���߳�
void WriteData1(void);
void WriteData2(void);
void multiThread()
{
	cout << "��ӭʹ��GL4A�����߳�" << endl;
	cout << "�汾��" << GL4A_VERSION << endl;
	errHandle err = errHandle();
	cout << "��GL4A���г�ʼ��......��Ĭ�������ļ�·����" << GL4A_DEFAULT_CONFIG_DIR << endl;
	err = GL4A_init(GL4A_DEFAULT_CONFIG_DIR);
	if (err == 0)
	{
		cout << "��־���ʼ���ɹ�" << endl;
	}
	else
	{
		cout << "��־���ʼ��ʧ��" << endl;
	}
	thread WriteThread1(WriteData1);
	thread WriteThread2(WriteData2);
	size_t count = 999999;
	while (count--)
	{
		Sleep(5);
	}
	WriteThread1.detach();
	WriteThread2.detach();
	GL4A_close();
	genConfigDefault();
}

int main()
{
	// ����Ĭ�������ļ�
	genConfigDefault();
	// �����÷�
	baseUsage();
	//���������ļ�
	changeConfig();
	// ���߳�
	multiThread();
	// �˳�ʱ�ָ�Ĭ�������ļ�
	genConfigDefault();
    return 0;
}

void WriteData1(void)
{
	for (size_t i = 0; i < 1000000; i++)
	{
		Sleep(50);
		GL4A_TRACE_LOG("M0", __F__, __L__, "Thread1\n");
	}
}

void WriteData2(void)
{
	for (size_t i = 0; i < 1000000; i++)
	{
		Sleep(50);
		GL4A_TRACE_LOG("M0", __F__, __L__, "Thread2\n");
	}
}


