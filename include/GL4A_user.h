/*******************************************************************
* ģ�飺��־��¼
* ���ܣ����ڽ�����־��¼
* ���ߣ���ͥ��
***********************************************************************/
#ifndef __GL4A_USER_H__
#define __GL4A_USER_H__

#include "./GL4A/GL4A_sink.h"

#ifndef GL4A_VERSION
#define GL4A_VERSION "V1.0.000"
#endif // !GL4A_VERSION

// Ĭ�ϵ���־�����ļ�·��
#ifndef GL4A_DEFAULT_CONFIG_DIR
#define GL4A_DEFAULT_CONFIG_DIR "../configFile/GL4A_config.cfg"
#endif // !GL4A_DEFAULT_CONFIG_DIR

namespace GL4A_Impl
{
	GL4A_INLINE json_J genConfig_Impl(void)
	{
		TotalCfgInfos logTotal;
		string fileStr = "../log/";
		{
			// console
			//logTotal.setConsoleState(false);
			//logTotal.setRollingState(false);
			//logTotal.setRollingDir_(fileStr + "/rolling/");

			logTotal.S_sinksMulti.S_consoleCfg.V_open = false;
			logTotal.S_sinksMulti.S_consoleCfg.V_color = false;
			// rolling
			logTotal.S_sinksMulti.S_rollingCfg.V_open = false;
			logTotal.S_sinksMulti.S_rollingCfg.V_dir = fileStr + "/rolling/";
			logTotal.S_sinksMulti.S_rollingCfg.V_count = 100;
			logTotal.S_sinksMulti.S_rollingCfg.V_size = 20 * 1024 * 1024;
			// daily
			logTotal.S_sinksMulti.S_dailyCfg.V_open = true;
			logTotal.S_sinksMulti.S_dailyCfg.V_dir = fileStr + "/daily/";
			logTotal.S_sinksMulti.S_dailyCfg.V_hour = 00;
			logTotal.S_sinksMulti.S_dailyCfg.V_minute = 00;
		}

		size_t modelCount = 1;
		logTotal.S_modelInfos.resize(modelCount);
		for (size_t i = 0; i < modelCount; i++)
		{
			logTotal.S_modelInfos[i].V_open = true;
			logTotal.S_modelInfos[i].V_name = "M" + to_string(i);
		}


		size_t sinksCount = 5;
		assert(sinksCount < GL4A_lv::off);
		logTotal.S_sinksInfos.resize(sinksCount);

		for (size_t i = 0; i < sinksCount; i++)
		{
			if ((0 == i) || (4 == i))
				logTotal.S_sinksInfos[i].V_open = true;
			else
				logTotal.S_sinksInfos[i].V_open = false;
			logTotal.S_sinksInfos[i].V_name = level::to_string_view((GL4A_lv)i).data();
		}

		json_J j;

		try
		{
			j = logTotal.to_json();
		}
		catch (const errThrow& e)
		{
			throw errThrow::create(1, e.what() + string("\n"));
		}
		return j;
	}

	GL4A_INLINE TotalCfgInfos genConfigStruct_Impl()
	{
		TotalCfgInfos logTotal;
		string fileStr = "../log/";
		{
			// console
			logTotal.S_sinksMulti.S_consoleCfg.V_open = true;
			logTotal.S_sinksMulti.S_consoleCfg.V_color = true;
			// rolling
			logTotal.S_sinksMulti.S_rollingCfg.V_open = true;
			logTotal.S_sinksMulti.S_rollingCfg.V_dir = fileStr + "/rolling/";
			logTotal.S_sinksMulti.S_rollingCfg.V_count = 100;
			logTotal.S_sinksMulti.S_rollingCfg.V_size = 100 * 1024 * 1024;
			// daily
			logTotal.S_sinksMulti.S_dailyCfg.V_open = true;
			logTotal.S_sinksMulti.S_dailyCfg.V_dir = fileStr + "/daily/";
			logTotal.S_sinksMulti.S_dailyCfg.V_hour = 00;
			logTotal.S_sinksMulti.S_dailyCfg.V_minute = 00;
		}

		size_t modelCount = 5;

		logTotal.S_modelInfos.resize(modelCount);
		for (size_t i = 0; i < modelCount; i++)
		{
			logTotal.S_modelInfos[i].V_open = true;
			logTotal.S_modelInfos[i].V_name = "M" + to_string(i);
		}


		size_t sinksCount = 5;
		logTotal.S_sinksInfos.resize(sinksCount);

		for (size_t i = 0; i < sinksCount; i++)
		{
			logTotal.S_sinksInfos[i].V_open = true;
			logTotal.S_sinksInfos[i].V_name = level::to_string_view((GL4A_lv)i).data();
		}
		return logTotal;
	}

	GL4A_INLINE json_J genConfig(void)
	{
		json_J j;
		GL4A_initCriticalLog();
		try
		{
			j = genConfig_Impl();
		}
		catch (const errThrow &e)
		{
			get("critical")->critical("Could not open config file! ->> {}\n", e.what());
		}
		GL4A_closeCriticalLog();
		return j;
	}

	GL4A_INLINE void GL4A_init_Impl(const char *dir)
	{
		json_J jOut;
		GL4A_initCriticalLog();
		// ���������ļ�
		try
		{
			jOut = loadCfgFile(dir);

		}
		catch (const errThrow &e)
		{
			get("critical")->critical("Could not open config file! ->> {}\n", e.what());
		}
		if (jOut.empty())
		{
			get("critical")->critical("Could not open config file! ->> {}\n");
			return;
		}

		TotalCfgInfos cfg;
		sinksRecCfgInfos S_sinksMulti;
		vector<sinksCfgInfos> S_sinksInfos;
		vector<modelsCfgInfos> S_modelInfos;

		try
		{
			cfg.from_json(jOut);
			S_sinksMulti = cfg.S_sinksMulti;
			S_sinksInfos.assign(cfg.S_sinksInfos.begin(), cfg.S_sinksInfos.end());
			S_modelInfos.assign(cfg.S_modelInfos.begin(), cfg.S_modelInfos.end());
		}
		catch (const errThrow &e)
		{
			get("critical")->critical("Could not make config struct from json! ->> {}\n", e.what());
		}

		GL4A_sinksMatrix S_sinkMatrix(S_sinksInfos.size(), 3);

		for (size_t j = 0; j < S_sinkMatrix.sinksCount; j++)
		{
			GL4A_sinks S_sinks;
			try
			{
				S_sinksMulti.S_rollingCfg.V_dir = cfg.S_sinksMulti.S_rollingCfg.V_dir + S_sinksInfos.at(j).V_name + ".log";
				S_sinksMulti.S_dailyCfg.V_dir = cfg.S_sinksMulti.S_dailyCfg.V_dir + S_sinksInfos.at(j).V_name + ".log";
				S_sinks.createSinks(S_sinksMulti);

			}
			catch (const errThrow &e)
			{
				get("critical")->critical("Could not create sinks from config! ->> {}\n", e.what());
			}

			S_sinkMatrix.set_value(j, S_sinks.to_vector());

			for (size_t i = 0; i < S_sinkMatrix.sinksType; i++)
			{
				if ((true == S_sinksInfos.at(j).V_open) && (true == S_sinksMulti.to_open().at(i)))
				{
					S_sinkMatrix.set_level(j, i, GL4A_lv(j));
					
				}
				else
				{
					S_sinkMatrix.set_level(j, i, GL4A_lv::off);
				}
			}
		}


		for (size_t i = 0; i < S_modelInfos.size(); i++)
		{
			shared_ptr<logger> logger;
			vector<GL4A_sinkPtr> sFlat = S_sinkMatrix.to_flat();
			try
			{
				logger = make_shared<spdlog::logger>(S_modelInfos.at(i).V_name, sFlat.begin(), sFlat.end());
				logger->set_level(GL4A_lv::trace);
				logger->set_pattern("[ Time: %Y-%m-%d %T | %L | %v");
				if (true == S_modelInfos.at(i).V_open)
				{
					logger->set_level(GL4A_lv::trace);
				}
				else
				{
					logger->set_level(GL4A_lv::off);
				}

				if (nullptr == get(logger->name()))
				{
					register_logger(logger);
				}
				else
				{
					drop(logger->name());
					register_logger(logger);;
				}
			}
			catch (const errThrow &e)
			{
				get("critical")->critical("Could not create loggers from config! ->> {}\n", e.what());
			}
		}
	}

	GL4A_INLINE void GL4A_reloadConfig_Impl(const char *dir)
	{
		try
		{
			GL4A_init_Impl(dir);
		}
		catch (const errThrow &e)
		{
			throw errThrow::create(1, string("Could not reload config file! ->> ") + e.what() + "\n");
		}
	}
}

/*******************************************************************
* namespace:
* GL4A
* Description:
* �������ռ������û�����
***********************************************************************/
namespace GL4A
{
	// �����¼
	class errHandle
	{
	public:
		errHandle() :code(0), msg("") {}

		errHandle(const size_t &i, const string &s) :code(i), msg(s) {}

		errHandle(const size_t &i) :code(i) {}

		virtual ~errHandle() {}

		void setErr(const size_t &i, const string &s)
		{
			code = i;
			msg = s;
		}

		void getErr(size_t &i, string &s) const
		{
			i = code;
			s = msg;
		}

		void clear()
		{
			code = 0;
			msg = "";
		}

		bool operator == (const size_t &i)const
		{
			return code == i;
		}

		bool operator != (const size_t &i)const
		{
			return code != i;
		}

		size_t getCode() const
		{
			return code;
		}

		string getMsg() const
		{
			return msg;
		}
	public:
	private:
		size_t code;
		string msg;
	};

	// �����ļ��ṹ
	typedef GL4A_Impl::TotalCfgInfos TotalCfgInfos;

	/*******************************************************************
	* Function:
	* errHandle genConfigDefault()
	* Description:
	* �˺������ڰ�Ĭ��ֵ���������ļ�, ��־·��GL4A_DEFAULT_CONFIG_DIR
	* Parameter:
	* -input:
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* 0 ���� �ɹ�
	* 1 ���� ʧ��
	***********************************************************************/
	GL4A_INLINE errHandle genConfigDefault(void)
	{

		errHandle err;
		json_J j_C;
		try
		{
			j_C = GL4A_Impl::genConfig_Impl();
			GL4A_Impl::saveCfgFile(GL4A_DEFAULT_CONFIG_DIR, j_C);
		}
		catch (const  GL4A_Impl::errThrow &e)
		{
			err.setErr(1, string("Could not create config file! ->>") + e.what() + "\n");
		}
		return err;
	}

	/*******************************************************************
	* Function:
	* errHandle saveCfgFileFromStruct(IN const char *dir, IN const TotalCfgInfos &S_cfg)
	* Description:
	* �˺������ڸ������ýṹ�屣��Ϊ�����ļ�
	* Parameter:
	* -input:
	* IN const char *dir							// ��־��¼�������ļ�Ŀ¼
	* IN const TotalCfgInfos &S_cfg					// ��־������ṹ��
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* 0 ���� �ɹ�
	* 1 ���� ʧ��
	***********************************************************************/
	GL4A_INLINE errHandle saveCfgFileFromStruct(IN const char *dir, IN const TotalCfgInfos &S_cfg)
	{
		errHandle err;
		try
		{
			GL4A_Impl::saveCfgFile(dir, S_cfg);
		}
		catch (const GL4A_Impl::errThrow &e)
		{
			err.setErr(1, string("Could not save config file! ->>") + e.what() + "\n");
		}
		return err;
	}

	/*******************************************************************
	* Function:
	* errHandle loadCfgFileToStruct(IN const char *dir, OUT const TotalCfgInfos &S_cfg)
	* Description:
	* �˺������ڴ������ļ��л�ȡ��־����
	* Parameter:
	* -input:
	* IN const char *dir							// ��־��¼�������ļ�Ŀ¼
	* -inputoutput:
	* none
	* -output:
	* OUT const TotalCfgInfos &S_cfg					// ��־������ṹ��
	* ReturnCode��
	* 0 ���� �ɹ�
	* 1 ���� ʧ��
	***********************************************************************/
	GL4A_INLINE errHandle loadCfgFileToStruct(IN const char *dir, OUT TotalCfgInfos &S_cfg)
	{
		errHandle err;
		try
		{
			S_cfg = GL4A_Impl::loadCfgFileStruct(dir);
		}
		catch (const GL4A_Impl::errThrow &e)
		{
			err.setErr(1, string("Could not load config file! ->>") + e.what() + "\n");
		}
		return err;
	}

	/*******************************************************************
	* Function:
	* errHandle GL4A_init(IN const char *dir)
	* Description:
	* �˺������ڳ�ʼ����־��¼����������һ�μ���
	* Parameter:
	* -input:
	* IN const char *dir							// ��־��¼�������ļ�Ŀ¼
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* 0 ���� �ɹ�
	* 1 ���� ʧ��
	***********************************************************************/
	GL4A_INLINE errHandle GL4A_init(IN const char *dir)
	{
		errHandle err;
		try
		{
			GL4A_Impl::GL4A_init_Impl(dir);
		}
		catch (const GL4A_Impl::errThrow &e)
		{

			err.setErr(1, string("Could not open config file! ->> ") + e.what() + "\n");
		}
		return err;
	}

	/*******************************************************************
	* Function:
	* errHandle GL4A_reloadConfig(IN const char *dir)
	* Description:
	* �˺����������¼��������ļ���֧��ʵʱ�ؼ���
	* Parameter:
	* -input:
	* IN const char *dir							// ��־��¼�������ļ�Ŀ¼
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* 0 ���� �ɹ�
	* 1 ���� ʧ��
	***********************************************************************/
	GL4A_INLINE errHandle GL4A_reloadConfig(IN const char *dir)
	{
		errHandle err;
		try
		{
			GL4A_Impl::GL4A_reloadConfig_Impl(dir);
		}
		catch (const GL4A_Impl::errThrow &e)
		{

			err.setErr(1, string("Could not reload config file! ->> ") + e.what() + "\n");
		}
		return err;
	}

	/*******************************************************************
	* Function:
	* void GL4A_close()
	* Description:
	* �˺������ڹر���־��¼�����ڳ��������˳�ʱ����
	* Parameter:
	* -input:
	* none
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* none
	***********************************************************************/
	void GL4A_close()
	{
		drop_all();
	}

	/*******************************************************************
	* Function:
	* void GL4A_TRACE_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	* Description:
	* �˺������ڼ�¼TRACE
	* Parameter:
	* -input:
	* IN const char* str				// �������
	* IN const char* file				// �ļ�����һ����_FILE_
	* IN const unsigned int &line       // �кţ�һ����_LINE_
	* IN const char * sz_format         // ��ʽ�����������ͬprintf
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* none
	***********************************************************************/
	void GL4A_TRACE_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	{
		shared_ptr<logger> logger = get(str);
		// ����
		char Text[MAX_TEXT_LENGTH];
		int nRet = 0;
		RELEASE_FORMAT(nRet, Text, MAX_TEXT_LENGTH, sz_format);

		if (NULL != logger)
		{
			logger->trace("{}", Out_stream{ (string)file ,line, (string)str ,(string)Text });
		}
		else
			get("critical")->critical("Model is non-exist! ->> {}", str);
	}

	/*******************************************************************
	* Function:
	* void GL4A_DEBUG_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	* Description:
	* �˺������ڼ�¼DEBUG
	* Parameter:
	* -input:
	* IN const char* str				// �������
	* IN const char* file				// �ļ�����һ����_FILE_
	* IN const unsigned int &line       // �кţ�һ����_LINE_
	* IN const char * sz_format         // ��ʽ�����������ͬprintf
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* none
	***********************************************************************/
	void GL4A_DEBUG_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	{
		shared_ptr<logger> logger = get(str);
		// ����
		char Text[MAX_TEXT_LENGTH];
		int nRet = 0;
		RELEASE_FORMAT(nRet, Text, MAX_TEXT_LENGTH, sz_format);

		if (NULL != logger)
		{
			logger->debug("{}", Out_stream{ (string)file ,line, (string)str ,(string)Text });
		}
		else
			get("critical")->critical("Model is non-exist! ->> {}", str);
	}

	/*******************************************************************
	* Function:
	* void GL4A_INFO_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	* Description:
	* �˺������ڼ�¼INFO
	* Parameter:
	* -input:
	* IN const char* str				// �������
	* IN const char* file				// �ļ�����һ����_FILE_
	* IN const unsigned int &line       // �кţ�һ����_LINE_
	* IN const char * sz_format         // ��ʽ�����������ͬprintf
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* none
	***********************************************************************/
	void GL4A_INFO_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	{
		shared_ptr<logger> logger = get(str);
		// ����
		char Text[MAX_TEXT_LENGTH];
		int nRet = 0;
		RELEASE_FORMAT(nRet, Text, MAX_TEXT_LENGTH, sz_format);

		if (NULL != logger)
		{
			logger->info("{}", Out_stream{ (string)file ,line, (string)str ,(string)Text });
		}
		else
			get("critical")->critical("Model is non-exist! ->> {}", str);
	}

	/*******************************************************************
	* Function:
	* void GL4A_WARN_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	* Description:
	* �˺������ڼ�¼WARN
	* Parameter:
	* -input:
	* IN const char* str				// �������
	* IN const char* file				// �ļ�����һ����_FILE_
	* IN const unsigned int &line       // �кţ�һ����_LINE_
	* IN const char * sz_format         // ��ʽ�����������ͬprintf
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* none
	***********************************************************************/
	void GL4A_WARN_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	{
		shared_ptr<logger> logger = get(str);
		// ����
		char Text[MAX_TEXT_LENGTH];
		int nRet = 0;
		RELEASE_FORMAT(nRet, Text, MAX_TEXT_LENGTH, sz_format);

		if (NULL != logger)
		{
			logger->warn("{}", Out_stream{ (string)file ,line, (string)str ,(string)Text });
		}
		else
			get("critical")->critical("Model is non-exist! ->> {}", str);
	}

	/*******************************************************************
	* Function:
	* void GL4A_ERROR_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	* Description:
	* �˺������ڼ�¼ ERROR
	* Parameter:
	* -input:
	* IN const char* str				// �������
	* IN const char* file				// �ļ�����һ����_FILE_
	* IN const unsigned int &line       // �кţ�һ����_LINE_
	* IN const char * sz_format         // ��ʽ�����������ͬprintf
	* -inputoutput:
	* none
	* -output:
	* none
	* ReturnCode��
	* none
	***********************************************************************/
	void GL4A_ERROR_LOG(IN const char* str, IN const char* file, IN const unsigned int &line, IN const char * sz_format, ...)
	{
		shared_ptr<logger> logger = get(str);

		// ����
		char Text[MAX_TEXT_LENGTH];
		int nRet = 0;
		RELEASE_FORMAT(nRet, Text, MAX_TEXT_LENGTH, sz_format);

		if (NULL != logger)
		{
			//logger->set_level(GL4A_lv::err);
			logger->error("{}", Out_stream{ (string)file ,line, (string)str ,(string)Text });
			logger->flush();

		}
		else
			get("critical")->critical("Model is non-exist! ->> {}", str);
	}
}

#endif __GL4A_USER_H__

