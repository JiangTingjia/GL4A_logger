/*******************************************************************
* 模块：日志记录
* 功能：用于进行日志记录
* 作者：蒋庭佳
***********************************************************************/
#ifndef __GL4A_SINK_H__
#define __GL4A_SINK_H__

// 包含内部头文件
#include "./GL4A_config.h"
#include "../spdlog/spdlog.h"
#include "../spdlog/sinks/stdout_color_sinks.h"
#include "../spdlog/sinks/rotating_file_sink.h"
#include "../spdlog/sinks/daily_file_sink.h"
#include "../spdlog/fmt/ostr.h"
// 包含外部头文件

// 引用命名空间 
using namespace spdlog;
using namespace sinks;
//using namespace level;

// 定义宏
#ifndef MAX_TEXT_LENGTH
#define MAX_TEXT_LENGTH 1024
#endif // !MAX_TEXT_LENGTH

#ifndef __F__
#define __F__ __FILE__
#endif // !_F_

#ifndef __L__
#define __L__ __LINE__
#endif // !_L_

#ifndef RELEASE_FORMAT
#define RELEASE_FORMAT(nPrintLength,szBuf,nBufferSize,szFormat)\
		{\
			va_list pArgList; \
			va_start(pArgList, szFormat); \
			nPrintLength += vsprintf_s(szBuf + nPrintLength, \
				nBufferSize - nPrintLength, szFormat, pArgList); \
			va_end(pArgList); \
			if (nPrintLength>(nBufferSize - 1)) nPrintLength = nBufferSize - 1; \
			*(szBuf + nPrintLength) = '\0'; \
		}
#endif // !RELEASE_FORMAT

// 定义结构体
typedef level::level_enum GL4A_lv;
typedef sink_ptr GL4A_sinkPtr;

struct Out_stream
{
	string File;
	unsigned int LINE;
	string str;
	string Text;
	template<typename OStream>
	friend OStream& operator<<(OStream& os, const Out_stream &c)
	{
		if (strstr(c.File.c_str(), "\\") != NULL)
		{
			return os << " F/L: " << strrchr(c.File.c_str(), '\\') + 1 << "/" << c.LINE << " | " << "Mode:" << c.str << " ]\n" << "[ Text --> " << c.Text << " ]\n";
		}
		else
		{
			return os << " F/L: " << c.File << "/" << c.LINE << " | " << "Mode:" << c.str << " ]\n" << "[ Text --> " << c.Text << " ]\n";
		}
	}
};

// 
namespace GL4A_Impl
{
	class GL4A_sinks_Impl
	{
	public:
	public:
	};


	class GL4A_sinks
	{
	public:
		GL4A_sinks(){}

		GL4A_sinks(const GL4A_sinkPtr &S_i, const GL4A_sinkPtr &S_j, const GL4A_sinkPtr &S_k)
		:consoleSink(S_i), rollingSink(S_j), dailySink(S_k){}

		GL4A_sinks(const consoleCfgInfos &S_i, const rollingCfgInfos &S_j, const dailyCfgInfos &S_k)
		{
			try
			{
				createSinks_Impl(S_i, S_j, S_k);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Init sinks failed!") + e.what() + "\n");
			}
		}

		GL4A_sinks(const sinksRecCfgInfos &S_i)
		{
			try
			{
				createSinks_Impl(S_i);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Init sinks failed!") + e.what() + "\n");
			}
		}

		virtual ~GL4A_sinks() {}

		void createSink(const consoleCfgInfos &console)
		{
			createSink_Impl(console);
			return ;
		}
		void createSink(const rollingCfgInfos &rolling)
		{
			createSink_Impl(rolling);
			return;
		}
		void createSink(const dailyCfgInfos &daily)
		{
			createSink_Impl(daily);
			return;
		}
		// 创建一个 sink 集合
		void createSinks(const consoleCfgInfos &c, const rollingCfgInfos &r, const dailyCfgInfos &d)
		{
			try
			{
				createSink(c);
				createSink(r);
				createSink(d);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Create sinks failed!") + e.what() + "\n");
			}
		}

		void createSinks(const sinksRecCfgInfos &i)
		{
			createSinks(i.S_consoleCfg, i.S_rollingCfg, i.S_dailyCfg);
			return;
		}

		vector<GL4A_sinkPtr> to_vector() const
		{
			return vector<GL4A_sinkPtr> ({ consoleSink , rollingSink , dailySink });
	
		}
	private:
		void createSink_Impl(const consoleCfgInfos &console)
		{
			try
			{
				sink_ptr sinkTmp = nullptr;
				sinkTmp = make_shared<stdout_color_sink_mt>();
				if (true == console.V_open)
				{
					sinkTmp->set_level(GL4A_lv::trace);
				}
				else
				{
					sinkTmp->set_level(GL4A_lv::off);
				}
				consoleSink.swap(sinkTmp);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Create sink failed!") + e.what() + "\n");
			}

		}

		void createSink_Impl(const rollingCfgInfos &rolling)
		{
			try
			{
				sink_ptr sinkTmp = nullptr;
				createDir(rolling.V_dir.c_str());
				sinkTmp = make_shared<rotating_file_sink_mt>(rolling.V_dir, rolling.V_size, rolling.V_count);
				if (true == rolling.V_open)
				{
					sinkTmp->set_level(GL4A_lv::trace);
				}
				else
				{
					sinkTmp->set_level(GL4A_lv::off);
				}
				rollingSink.swap(sinkTmp);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Create sink failed!") + e.what() + "\n");
			}
		}

		void createSink_Impl(const dailyCfgInfos &daily)
		{
			try
			{
				sink_ptr sinkTmp = nullptr;
				createDir(daily.V_dir.c_str());
				sinkTmp = make_shared<daily_file_sink_mt>(daily.V_dir, (int)daily.V_hour, (int)daily.V_minute);
				if (true == daily.V_open)
				{
					sinkTmp->set_level(GL4A_lv::trace);
				}
				else
				{
					sinkTmp->set_level(GL4A_lv::off);
				}
				dailySink.swap(sinkTmp);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Create sink failed!") + e.what() + "\n");
			}
		}

		void createSinks_Impl(const consoleCfgInfos &c, const rollingCfgInfos &r, const dailyCfgInfos &d)
		{
			try
			{
				createSink(c);
				createSink(r);
				createSink(d);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Create sinks failed!") + e.what() + "\n");
			}
		}

		void createSinks_Impl(const sinksRecCfgInfos &i)
		{
			try
			{
				createSinks_Impl(i.S_consoleCfg, i.S_rollingCfg, i.S_dailyCfg);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Create sinks failed!") + e.what() + "\n");
			}
		}

	public:
		GL4A_sinkPtr consoleSink;
		GL4A_sinkPtr rollingSink;
		GL4A_sinkPtr dailySink;
	};

	class GL4A_sinksMatrix
	{
	public:
		GL4A_sinksMatrix() {}
		GL4A_sinksMatrix(const size_t &S_c, const size_t &s_T) :sinksCount(S_c), sinksType(s_T)
		{
			sMatrix.resize(sinksCount);
			for (size_t i = 0; i < sinksCount; i++)
			{
				sMatrix[i].resize(sinksType);
			}
		}

		GL4A_sinksMatrix(const vector<GL4A_sinks> &S_i): sinksCount(S_i.size()), sinksType(3)
		{
			sMatrix.resize(sinksCount);
			for (size_t i = 0; i < sinksCount; i++)
			{
				sMatrix[i].resize(sinksType);
				sMatrix[i].assign(S_i.at(i).to_vector().begin(), S_i.at(i).to_vector().end());

			}
		}

		virtual ~GL4A_sinksMatrix() {}

		void add(const GL4A_sinks &S_i)
		{
			sinksCount++;
			sMatrix.push_back(S_i.to_vector());
		}

		void set_value(const size_t &i_r, const vector<GL4A_sinkPtr> &S_value)
		{
			assert(i_r < sinksCount);
			sMatrix[i_r].assign(S_value.begin(), S_value.end());
		}

		void set_level(const size_t &i_r, const size_t &i_c, const GL4A_lv &s_Lv)
		{
			assert(i_c < 3);
			sMatrix[i_r][i_c]->set_level(s_Lv);
		}

		void set_level(const size_t &i_r, const GL4A_lv &s_Lv)
		{
			for (size_t i = 0; i < sMatrix[i_r].size(); i++)
			{
				sMatrix[i_r][i]->set_level(s_Lv);
			}	
		}

		void set_level(const size_t &i_c)
		{
			for (size_t i = 0; i < sMatrix.size(); i++)
			{
				sMatrix[i][i_c]->set_level(GL4A_lv(i));
			}
		}

		void set_level(const vector<vector<bool>> &i_c)
		{
			for (size_t i = 0; i < sMatrix.size(); i++)
			{
				for (size_t j = 0; j < sMatrix[i].size(); j++)
				{
					if(true == i_c[i][j])
						sMatrix[i][j]->set_level(GL4A_lv(i));
					else
						sMatrix[i][j]->set_level(GL4A_lv::off);
				}
			}
		}

		vector<GL4A_sinkPtr> to_flat() const
		{
			size_t count = 0;
			vector<GL4A_sinkPtr> Tmp(sinksCount* sinksType);
			for (size_t i = 0; i < sinksCount; i++)
			{
				for (size_t j = 0; j < sinksType; j++)
				{
					Tmp[count] = sMatrix[i][j];
					count++;
				}
			}
			return Tmp;
		}
	public:
		size_t sinksCount;
		size_t sinksType;
		vector<vector<GL4A_sinkPtr>> sMatrix;
	};

	void GL4A_initCriticalLog(void)
	{
		const char *dir = "../log/critical/critical.log";
		string pattern = "[ Time: %Y-%m-%d %T ] %v";
		sink_ptr sinkTmp = nullptr;
		shared_ptr<logger> logger;
		createDir(dir);
		sinkTmp = make_shared<daily_file_sink_mt>(dir, 00, 00);
		sinkTmp->set_level(GL4A_lv::critical);
		logger = make_shared<spdlog::logger>("critical", sinkTmp);
		logger->set_level(GL4A_lv::critical);
		logger->set_pattern(pattern);
		logger->flush_on(GL4A_lv::critical);

		
		if (nullptr == get(logger->name()))
		{
			register_logger(logger);
		}
		else
		{
			//drop(logger->name());
			//register_logger(logger);;
		}
	}

	void GL4A_closeCriticalLog(void)
	{
		if (NULL != get("critical"))
		{
			drop("critical");
		}
	}

}
#endif // !__GL4A_SINK_H__

