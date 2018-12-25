
#include "../include/GL4A_user.h"

using namespace GL4A;

// 基本用法
void baseUsage()
{
	cout << "欢迎使用GL4A：基本用法" << endl;
	cout << "版本：" << GL4A_VERSION << endl;
	errHandle err = errHandle();
	// 初始化日志库（在程序入口处）
	cout << "对GL4A进行初始化......，默认配置文件路径：" << GL4A_DEFAULT_CONFIG_DIR << endl;
	err = GL4A_init(GL4A_DEFAULT_CONFIG_DIR);
	if (err == 0)
	{
		cout << "日志库初始化成功" << endl;
	}
	else
	{
		cout << "日志库初始化失败" << endl;
	}

	// 将 组件 M0 输出到 TRACE 和 ERROR
	// 在上一级目录下生成log文件夹，分别含有critical/rolling/daily三个文件夹
	// 由于默认配置文件中只开启了daily日志记录器，所有仅daily中记录了 trace 和 error
	GL4A_TRACE_LOG("M0", __F__, __L__, "TRACE 测试成功！\n");
	GL4A_ERROR_LOG("M0", __F__, __L__, "ERROR 测试成功！\n");

	// .... 继续输出

	// 关闭日志库（在程序退出时）
	cout << "日志库使用完毕!" << endl;
	GL4A_close();
	genConfigDefault();
}

// 更改配置文件
void changeConfig()
{
	
	cout << "欢迎使用GL4A：更改配置文件" << endl;
	cout << "版本：" << GL4A_VERSION << endl;
	errHandle err = errHandle();
	cout << "对GL4A进行初始化......，默认配置文件路径：" << GL4A_DEFAULT_CONFIG_DIR << endl;
	err = GL4A_init(GL4A_DEFAULT_CONFIG_DIR);
	if (err == 0)
	{
		cout << "日志库初始化成功" << endl;
	}
	else
	{
		cout << "日志库初始化失败" << endl;
	}

	// 输出M0组件 信息
	GL4A_TRACE_LOG("M0", __F__, __L__, "我好像成功了！\n");

	// 输出M1组件信息
	GL4A_TRACE_LOG("M1", __F__, __L__, "我好像失败了，为啥M0能成功，M1却不能成功，M1也想成功！\n");

	// 获取配置文件内容
	TotalCfgInfos cfg = TotalCfgInfos();
	err = loadCfgFileToStruct(GL4A_DEFAULT_CONFIG_DIR, cfg);

	// 调试不方便，要不也在console 控制台也输出一下吧
	bool consoleState = cfg.getConsoleState();
	cout << string("console state:") << ((true == consoleState) ? string("true") : string("false")) << endl;

	cfg.setConsoleState(true);

	// 查看配置文件中组件
	vector<pair<string, bool>> models = cfg.catchModels();
	for (size_t i = 0; i < models.size(); i++)
	{
		cout << "组件名:" << models.at(i).first << "状态:" << models.at(i).second << endl;
	}
	cout << "原来M1组件不再配置文件中！" << endl;

	// 我就想输出M1,我还想输出DOG 
	cfg.addModel("M1", true);
	cfg.addModel("DOG", true);

	// 保存一下配置文件，并重新加载一下
	saveCfgFileFromStruct(GL4A_DEFAULT_CONFIG_DIR, cfg);
	GL4A_reloadConfig(GL4A_DEFAULT_CONFIG_DIR);

	// 试一下,再console中会输出两条DOG信息，这是由于 ERROR等级比TRACE高，在高等级中输出的信息一定会在低等级中输出，这样子在TRACE中会保证数据的上下文连贯性
	GL4A_TRACE_LOG("M1", __F__, __L__, "我好像成功了\n");
	GL4A_ERROR_LOG("DOG", __F__, __L__, "我好像成功了\n");

	// 你还可以
	// 设置某个组件是否启用
	cfg.changeModelState("M0", false);
	// 清空所有组件
	cfg.clearModels();
	// 删除某个组件
	cfg.deleteModel("M0");
	// 获取console 开关状态
	bool state = cfg.getConsoleState();
	// ...具体参见 TotalCfgInfos 类 


	// 关闭日志库（在程序退出时）
	cout << "日志库使用完毕!" << endl;
	GL4A_close();
}

// 多线程
void WriteData1(void);
void WriteData2(void);
void multiThread()
{
	cout << "欢迎使用GL4A：多线程" << endl;
	cout << "版本：" << GL4A_VERSION << endl;
	errHandle err = errHandle();
	cout << "对GL4A进行初始化......，默认配置文件路径：" << GL4A_DEFAULT_CONFIG_DIR << endl;
	err = GL4A_init(GL4A_DEFAULT_CONFIG_DIR);
	if (err == 0)
	{
		cout << "日志库初始化成功" << endl;
	}
	else
	{
		cout << "日志库初始化失败" << endl;
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
	// 生成默认配置文件
	genConfigDefault();
	// 基本用法
	baseUsage();
	//更改配置文件
	changeConfig();
	// 多线程
	multiThread();
	// 退出时恢复默认配置文件
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


