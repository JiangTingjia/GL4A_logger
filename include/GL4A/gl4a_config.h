/*******************************************************************
* ģ�飺��־��¼
* ���ܣ����ڽ�����־��¼
* ���ߣ���ͥ��
***********************************************************************/
#ifndef __GL4A_CONFIG_H__
#define __GL4A_CONFIG_H__

// Ӧ���ڲ�ͷ�ļ�
#include "../json/json.hpp"

// ����C++��׼
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <iomanip>
#include <io.h>

//���������ռ�
using namespace std;
using json = nlohmann::json;


// �궨��
#define IN
#define OUT
#define INOUT

#ifndef GL4A_INLINE
#define GL4A_INLINE static inline
#endif // !GL4A_INLINE

// �ṹ�嶨��
typedef json json_J;

namespace GL4A_Impl
{
	// �쳣������ 
	class errThrow : public std::exception
	{
	public:
		static errThrow create(int id_, const std::string& what_arg)
		{
			std::string w = name("error->", id_) + ": " + what_arg + "\n";
			return errThrow(id_, w.c_str());
		}

		static errThrow create(int id_, const size_t &iLine, const std::string& what_arg)
		{
			std::string w = name("error->", id_) + ": Line: " + to_string(iLine) + what_arg + "\n";
			return errThrow(id_, w.c_str());
		}

		/// returns the explanatory string
		const char* what() const noexcept override
		{
			return m.what();
		}

		/// the id of the exception
		const int id;

	protected:
		errThrow(int id_, const char* what_arg) : id(id_), m(what_arg) {}

		static std::string name(const std::string& ename, int id_)
		{
			return "[json.exception." + ename + "." + std::to_string(id_) + "] ";
		}

	private:
		std::runtime_error m;
	};

	// console��־��¼��������
	class consoleCfgInfos
	{
	public:
		consoleCfgInfos() :V_open(false), V_color(false) {}

		consoleCfgInfos(const bool &i, const bool &j) :V_open(i), V_color(j) {}

		consoleCfgInfos(const consoleCfgInfos &i) :V_open(i.V_open), V_color(i.V_color) {}

		virtual ~consoleCfgInfos(void) {}

		// ͨ�� json ��ʼ��
		consoleCfgInfos(const json_J &i)
		{
			try
			{
				from_json_Impl(i);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("consoleCfgInfos init failed!") + e.what() + "\n");
			}
		}

		// ת json
		json_J to_json(void) const
		{
			return to_json_Impl();
		}

		// �� json ��ȡ
		void from_json(const json_J &i)
		{
			from_json_Impl(i);
		}

	private:
		void from_json_Impl(const json_J &i)
		{
			if (i.find("V_open") == i.end())
			{
				throw errThrow::create(1, "Console config don't has a element: open\n");
			}

			if (i.find("V_color") == i.end())
			{
				throw errThrow::create(1, "Console config don't has a element: color\n");
			}

			try
			{
				V_open = i.at("V_open").get<bool>();
				V_color = i.at("V_color").get<bool>();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}
		}

		json_J to_json_Impl(void) const
		{
			json_J j;
			j["V_open"] = V_open;
			j["V_color"] = V_color;
			return j;
		}

	public:
		bool V_open;	// �Ƿ�����
		bool V_color;	// �Ƿ����ɫ��ӡ
	private:
		//
	};

	// ����С������־��¼��������
	class rollingCfgInfos
	{
	public:
		rollingCfgInfos() :V_open(false), V_dir(""), V_size(0), V_count(0) {}

		rollingCfgInfos(const bool &i, const string &d, const size_t &j, const size_t &k) :V_open(i), V_dir(d), V_size(j), V_count(k) {}

		virtual ~rollingCfgInfos(void) {}

		rollingCfgInfos(const rollingCfgInfos &i) :V_open(i.V_open), V_dir(i.V_dir), V_size(i.V_size), V_count(i.V_count) {}

		rollingCfgInfos(const json_J &i)
		{
			try
			{
				from_json_Impl(i);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("rollingCfgInfos init failed!") + e.what() + "\n");
			}
		}

		// ת json
		json_J to_json(void) const
		{
			return to_json_Impl();
		}

		// �� json ��ȡ
		void from_json(const json_J &i)
		{
			from_json_Impl(i);
		}
	private:
		json_J to_json_Impl() const
		{
			json_J j;
			j["V_open"] = V_open;
			j["V_dir"] = V_dir;
			j["V_size"] = V_size;
			j["V_count"] = V_count;
			return j;
		}

		// jsonת�ṹ��
		void from_json_Impl(const json_J &i)
		{
			if (i.find("V_open") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: open\n");
			}

			if (i.find("V_dir") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: dir\n");
			}

			if (i.find("V_size") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: size\n");
			}

			if (i.find("V_count") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: count\n");
			}

			try
			{
				V_open = i["V_open"].get<bool>();
				V_dir = i["V_dir"].get<string>();
				V_size = i["V_size"].get<size_t>();
				V_count = i["V_count"].get<size_t>();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}

			return;
		}
	public:
		bool V_open;					// �Ƿ�����
		string V_dir;					// �ļ����Ŀ¼
		size_t V_size;			// ÿ���ļ������size
		size_t V_count;			// �����ڵ��ļ�����
	};

	// ���������־��¼��������
	class dailyCfgInfos
	{
	public:
		dailyCfgInfos() :V_open(0), V_dir(""), V_hour(0), V_minute(0) {}

		dailyCfgInfos(const bool &i, const string d, const unsigned int &j, const unsigned int &k) :V_open(i), V_dir(d), V_hour(j), V_minute(k) {}

		virtual ~dailyCfgInfos(void) {}

		dailyCfgInfos(const json_J &i)
		{
			if (i.find("V_open") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: open\n");
			}

			if (i.find("V_dir") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: dir\n");
			}

			if (i.find("V_hour") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: V_hour\n");
			}

			if (i.find("V_minute") == i.end())
			{
				throw errThrow::create(1, "rolling config don't has a element: V_minute\n");
			}

			try
			{
				V_open = i["V_open"].get<bool>();
				V_dir = i["V_dir"].get<string>();
				V_hour = i["V_hour"].get<size_t>();
				V_minute = i["V_minute"].get<size_t>();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}

		}

		json_J to_json(void) const
		{
			return to_json_Impl();
		}

		void from_json(const json_J &i)
		{
			from_json_Impl(i);
		}

	private:
		json_J to_json_Impl(void) const
		{
			json_J j;
			j["V_open"] = V_open;
			j["V_dir"] = V_dir;
			j["V_hour"] = V_hour;
			j["V_minute"] = V_minute;
			return j;
		}
		// jsonת�ṹ��
		void from_json_Impl(const json_J &i)
		{
			if (i.find("V_open") == i.end())
			{
				throw errThrow::create(1, "daily config don't has a element: open\n");
			}

			if (i.find("V_dir") == i.end())
			{
				throw errThrow::create(1, "daily config don't has a element: dir\n");
			}

			if (i.find("V_hour") == i.end())
			{
				throw errThrow::create(1, "daily config don't has a element: hour\n");
			}

			if (i.find("V_minute") == i.end())
			{
				throw errThrow::create(1, "daily config don't has a element: minute\n");
			}

			try
			{
				V_open = i["V_open"].get<bool>();
				V_dir = i["V_dir"].get<string>();
				V_hour = i["V_hour"].get<size_t>();
				V_minute = i["V_minute"].get<size_t>();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}

			return;
		}
	public:
		bool V_open;			// �Ƿ�����
		string V_dir;			// �ļ����Ŀ¼
		size_t V_hour;			// ������hour
		size_t V_minute;			// ������minute
	};

	// ���ּ�¼�������ü���
	class sinksRecCfgInfos
	{
	public:
		sinksRecCfgInfos() {}

		sinksRecCfgInfos(const consoleCfgInfos &i, const rollingCfgInfos &j, const dailyCfgInfos &k) :S_consoleCfg(i), S_rollingCfg(j), S_dailyCfg(k) {}

		virtual ~sinksRecCfgInfos(void) {}

		sinksRecCfgInfos(const json_J &i)
		{
			try
			{
				from_json_Impl(i);
			}
			catch (const errThrow& e)
			{
				throw errThrow::create(2, string("Catch total sink config failed!") + e.what() + "\n");
			}
		}

		json_J to_json(void) const
		{
			return to_json_Impl();
		}

		void from_json(const json_J &i)
		{
			from_json_Impl(i);
		}

		vector<bool> to_open() const
		{
			return vector<bool>({ S_consoleCfg.V_open, S_rollingCfg.V_open, S_dailyCfg.V_open });
		}
	private:
		json_J to_json_Impl(void) const
		{
			json_J j;
			try
			{
				j["S_consoleCfg"] = S_consoleCfg.to_json();
				j["S_rollingCfg"] = S_rollingCfg.to_json();
				j["S_dailyCfg"] = S_dailyCfg.to_json();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("Sinks config to json failed!") + e.what() + "\n");
			}

			return j;
		}

		// jsonת�ṹ��
		void from_json_Impl(const json_J &i)
		{
			if (i.find("S_consoleCfg") == i.end())
			{
				throw errThrow::create(1, "sinksRecCfgInfos config don't has a element: console\n");
			}

			if (i.find("S_rollingCfg") == i.end())
			{
				throw errThrow::create(1, "sinksRecCfgInfos config don't has a element: rolling\n");
			}

			if (i.find("S_dailyCfg") == i.end())
			{
				throw errThrow::create(1, "sinksRecCfgInfos config don't has a element: daily\n");
			}

			try
			{
				S_consoleCfg.from_json(i["S_consoleCfg"]);
				S_rollingCfg.from_json(i["S_rollingCfg"]);
				S_dailyCfg.from_json(i["S_dailyCfg"]);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}
			return;
		}

	public:
		consoleCfgInfos S_consoleCfg;				// ��־���Ŀ¼
		rollingCfgInfos S_rollingCfg;
		dailyCfgInfos S_dailyCfg;
	};

	// �������ƺͿ��� ��
	class nameAndOpen
	{
	public:
		nameAndOpen() :V_open(false), V_name("") {}

		nameAndOpen(const string &j, const bool &i) :V_open(i), V_name(j) {}

		virtual ~nameAndOpen() {}

		nameAndOpen(const json_J &i)
		{
			try
			{
				from_json_Impl(i);
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("modelsCfgInfos init failed!") + e.what() + "\n");
			}
		}

		json_J to_json(void) const
		{
			return to_json_Impl();
		}

		void from_json(const json_J &i)
		{
			from_json_Impl(i);
		}

	private:
		json_J to_json_Impl(void) const
		{
			json_J j;
			j["V_open"] = V_open;
			j["V_name"] = V_name;
			return j;
		}

		// jsonת�ṹ��
		void from_json_Impl(const json_J &i)
		{
			if (i.find("V_open") == i.end())
			{
				throw errThrow::create(1, "modelsCfgInfos config don't has a element: open\n");
			}

			if (i.find("V_name") == i.end())
			{
				throw errThrow::create(1, "modelsCfgInfos config don't has a element: name\n");
			}
			try
			{
				V_open = i["V_open"].get<bool>();
				V_name = i["V_name"].get<string>();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}
			return;
		}
	public:
		bool V_open;
		string V_name;
	};

	class nameAndOpenVector
	{
	public:
		json_J to_json() const
		{
			json_J j;
			for (size_t i = 0; i < S_modelInfos.size(); i++)
			{
				j[i] = S_modelInfos.at(i).to_json();
			}
			return j;
		}

		void from_json(const json_J &S_json)
		{
			S_modelInfos.resize(S_json.size());
			for (size_t i = 0; i < S_json.size(); i++)
			{
				S_modelInfos[i].from_json(S_json[i]);
			}
		}
	public:
		vector<nameAndOpen> S_modelInfos;
	};

	// ��¼�����������
	class modelsCfgInfos :public nameAndOpen
	{
	public:
		modelsCfgInfos() :nameAndOpen() {}

		modelsCfgInfos(const string &name, const bool &state) :nameAndOpen(name, state) {}

		bool operator==(const modelsCfgInfos &input)
		{
			return ((input.V_name == V_name) && (input.V_open == V_open));
		}
	};

	class modelsCfgInfosVec :public nameAndOpenVector
	{

	};

	class sinksCfgInfos :public nameAndOpen
	{
	};

	class sinksCfgInfosVec :public nameAndOpenVector
	{
	};

	// ��¼������
	class TotalCfgInfos
	{
	public:
		TotalCfgInfos() :S_sinksMulti(), S_sinksInfos(), S_modelInfos() {}

		TotalCfgInfos(const sinksRecCfgInfos &i, const vector<sinksCfgInfos> &j, const vector<modelsCfgInfos> &k)
			:S_sinksMulti(i), S_sinksInfos(j), S_modelInfos(k) {}

		TotalCfgInfos(const sinksRecCfgInfos &i, const sinksCfgInfosVec &j, const modelsCfgInfosVec &k)
			:S_sinksMulti(i), S_sinksVec(j), S_modelVec(k) {}

		virtual ~TotalCfgInfos()
		{

		}

		json_J to_json(void) const
		{
			return to_json_Impl();
		}

		void from_json(const json_J &i)
		{
			from_json_Impl(i);
		}

		size_t getModelsCount() const
		{
			return S_modelInfos.size();
		}

		vector<string> getModelsName() const
		{
			vector<string> result(S_modelInfos.size());
			for (size_t i = 0; i < S_modelInfos.size(); i++)
			{
				result[i] = S_modelInfos.at(i).V_name;
			}
			return result;
		}
		// �ⲿ���ܺ���
	public:
		/*******************************************************************
		* Function:
		* vector<pair<string,bool>> catchModels()
		* Description:
		* �˺������ڻ�ȡ���������/ģ������ƺͶ�Ӧ�Ŀ���״̬
		* Parameter:
		* -input:
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* vector<pair<string,bool>> ���� ���/ģ��Ķ��У�ÿһ��Ԫ�صĵ�һ��ֵδ��������ڶ���ֵδ��Ӧ�Ŀ���
		***********************************************************************/
		vector<pair<string,bool>> catchModels() const
		{
			vector<modelsCfgInfos> tmp = catchModels_Impl();
			vector<pair<string, bool>> result(tmp.size());
			for (size_t i = 0; i < tmp.size(); i++)
			{
				result[i] = { tmp.at(i).V_name, tmp.at(i).V_open };
			}
			return result;
		}

		/*******************************************************************
		* Function:
		* void clearModels()
		* Description:
		* �˺������ڽ�����������������
		* Parameter:
		* -input:
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void clearModels()
		{
			S_modelInfos.clear();
		}

		/*******************************************************************
		* Function:
		* void addModel(const string &name, const bool &state)
		* Description:
		* �˺������������������һ���������
		* Parameter:
		* -input:
		* const string &name			// �������
		* const bool &state				// ���״̬
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void addModel(const string &name, const bool &state)
		{
			S_modelInfos.push_back(modelsCfgInfos(name, state));
		}

		/*******************************************************************
		* Function:
		* void deleteModel(const string &name)
		* Description:
		* �˺���������������ɾ��һ���������
		* Parameter:
		* -input:
		* const string &name			// �������
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void deleteModel(const string &name)
		{
			if (0 != S_modelInfos.size())
				remove_if(S_modelInfos.begin(), S_modelInfos.end(), [&name](const modelsCfgInfos &i) {return (0 == _strcmpi(i.V_name.c_str(), name.c_str())); });
		}

		/*******************************************************************
		* Function:
		* void changeModelState(const string &name, const bool &state)
		* Description:
		* �˺������ڸı������ж���Ŀ���״̬
		* Parameter:
		* -input:
		* const string &name			// �������
		* const bool &state				// ���״̬
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void changeModelState(const string &name, const bool &state)
		{
			for (size_t i = 0; i < S_modelInfos.size(); i++)
			{
				if (0 == _strcmpi(S_modelInfos.at(i).V_name.c_str(), name.c_str()))
				{
					S_modelInfos[i].V_open = state;
					break;
				}
			}
		}

		/*******************************************************************
		* Function:
		* void setConsoleState(const bool &state)
		* Description:
		* �˺�����������console����̨��־��¼���Ƿ���
		* Parameter:
		* -input:
		* const bool &state				// ״̬  true ��������  false ����ر�
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setConsoleState(const bool &state)
		{
			S_sinksMulti.S_consoleCfg.V_open = state;
		}

		/*******************************************************************
		* Function:
		* bool setConsoleState()
		* Description:
		* �˺������ڻ�ȡconsole����̨��־��¼���Ƿ���
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* // ���״̬  true ��������  false ����ر�
		***********************************************************************/
		bool getConsoleState() const
		{
			return S_sinksMulti.S_consoleCfg.V_open;
		}

		/*******************************************************************
		* Function:
		* void setRollingState(const bool &state)
		* Description:
		* �˺�����������rolling�ļ���־��¼���Ƿ���
		* Parameter:
		* -input:
		* const bool &state				// ״̬  true ��������  false ����ر�
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setRollingState(const bool &state)
		{
			S_sinksMulti.S_rollingCfg.V_open = state;
		}

		/*******************************************************************
		* Function:
		* bool getRollingState()
		* Description:
		* �˺������ڻ�ȡrolling�ļ���־��¼���Ƿ���
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* // ״̬  true ��������  false ����ر�
		***********************************************************************/
		bool getRollingState() const
		{
			return S_sinksMulti.S_rollingCfg.V_open;
		}

		/*******************************************************************
		* Function:
		* void setRollingDir_(const string &dir)
		* Description:
		* �˺�����������rolling�ļ���־��¼�������Ŀ¼
		* Parameter:
		* -input:
		* const string &dir			// �ļ�Ŀ¼
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setRollingDir_(const string &dir)
		{
			S_sinksMulti.S_rollingCfg.V_dir = dir;
		}

		/*******************************************************************
		* Function:
		* string getRollingDir_()
		* Description:
		* �˺������ڻ�ȡrolling�ļ���־��¼�������Ŀ¼
		* Parameter:
		* -input:
		* none 
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* string 			// �ļ�Ŀ¼
		***********************************************************************/
		string getRollingDir_() const
		{
			return S_sinksMulti.S_rollingCfg.V_dir;
		}

		/*******************************************************************
		* Function:
		* void setDailyState(const bool &state)
		* Description:
		* �˺�����������daily�ļ���־��¼���Ƿ���
		* Parameter:
		* -input:
		* const bool &state				// ״̬  true ��������  false ����ر�
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setDailyState(const bool &state)
		{
			S_sinksMulti.S_dailyCfg.V_open = state;
		}

		/*******************************************************************
		* Function:
		* bool getDailyState()
		* Description:
		* �˺������ڻ�ȡdaily�ļ���־��¼���Ƿ���
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* // ״̬  true ��������  false ����ر�
		***********************************************************************/
		bool getDailyState() const
		{
			return S_sinksMulti.S_dailyCfg.V_open;
		}

		/*******************************************************************
		* Function:
		* void setDailyDir(const string &dir)
		* Description:
		* �˺�����������daily�ļ���־��¼�������Ŀ¼
		* Parameter:
		* -input:
		* const string &dir			// �ļ�Ŀ¼
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setDailyDir(const string &dir)
		{
			S_sinksMulti.S_dailyCfg.V_dir = dir;
		}

		/*******************************************************************
		* Function:
		* string getDailyDir()
		* Description:
		* �˺������ڻ�ȡdaily�ļ���־��¼�������Ŀ¼
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* string 			// �ļ�Ŀ¼
		***********************************************************************/
		string getDailyDir() const
		{
			return S_sinksMulti.S_dailyCfg.V_dir;
		}

		/*******************************************************************
		* Function:
		* void setDebugState(const bool &state)
		* Description:
		* �˺����������� debug �ȼ���־����״̬
		* Parameter:
		* -input:
		* const bool &state				// ״̬  true ��������  false ����ر�
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setDebugState(const bool &state)
		{
			setLevelState_Impl("debug", state);
		}

		/*******************************************************************
		* Function:
		* bool getDailyState()
		* Description:
		* �˺������ڻ�ȡ debug �ȼ���־����״̬
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* // ״̬  true ��������  false ����ر�
		***********************************************************************/
		bool getDebugState() const
		{
			return getLevelState_Impl("debug");
		}

		/*******************************************************************
		* Function:
		* void setInfoState(const bool &state)
		* Description:
		* �˺����������� info �ȼ���־����״̬
		* Parameter:
		* -input:
		* const bool &state				// ״̬  true ��������  false ����ر�
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setInfoState(const bool &state)
		{
			setLevelState_Impl("info", state);
		}

		/*******************************************************************
		* Function:
		* bool getInfoState()
		* Description:
		* �˺������ڻ�ȡ info �ȼ���־����״̬
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* // ״̬  true ��������  false ����ر�
		***********************************************************************/
		bool getInfoState() const
		{
			return getLevelState_Impl("info");
		}

		/*******************************************************************
		* Function:
		* void setWarnState(const bool &state)
		* Description:
		* �˺����������� warn �ȼ���־����״̬
		* Parameter:
		* -input:
		* const bool &state				// ״̬  true ��������  false ����ر�
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* none
		***********************************************************************/
		void setWarnState(const bool &state)
		{
			setLevelState_Impl("warn", state);
		}

		/*******************************************************************
		* Function:
		* bool getWarnState()
		* Description:
		* �˺������ڻ�ȡ warn �ȼ���־����״̬
		* Parameter:
		* -input:
		* none
		* -inputoutput:
		* none
		* -output:
		* none
		* ReturnCode��
		* // ״̬  true ��������  false ����ر�
		***********************************************************************/
		bool getWarnState() const
		{
			return getLevelState_Impl("warn");
		}

	private:
		// �趨Trace����
		void setTraceState(const bool &state)
		{
			setLevelState_Impl("trace", state);
		}

		// �趨Error����
		void setErrorState(const bool &state)
		{
			setLevelState_Impl("error", state);
		}

	private:

		vector<modelsCfgInfos> catchModels_Impl() const
		{
			return S_modelInfos;
		}

		void setLevelState_Impl(const string &name, const bool &state)
		{
			for (size_t i = 0; i < S_sinksInfos.size(); i++)
			{
				if (0 == _strcmpi(S_sinksInfos.at(i).V_name.c_str(), name.c_str()))
				{
					S_sinksInfos[i].V_open = state;
					break;
				}
			}
		}

		bool getLevelState_Impl(const string &name) const
		{
			for (size_t i = 0; i < S_sinksInfos.size(); i++)
			{
				if (0 == _strcmpi(S_sinksInfos.at(i).V_name.c_str(), name.c_str()))
				{
					return S_sinksInfos[i].V_open;
					break;
				}
			}
		}

		json_J to_json_Impl(void) const
		{
			json_J j;
			try
			{
				j["S_sinksMulti"] = S_sinksMulti.to_json();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("S_sinksMulti config to json failed!") + e.what() + "\n");
			}

			for (size_t i = 0; i < S_sinksInfos.size(); i++)
			{
				try
				{
					j["S_sinksInfos"].push_back(S_sinksInfos.at(i).to_json());
				}
				catch (const errThrow &e)
				{
					throw errThrow::create(1, string("S_sinksInfos config to json failed!") + e.what() + "\n");
				}
			}

			for (size_t i = 0; i < S_modelInfos.size(); i++)
			{
				try
				{
					j["S_modelInfos"].push_back(S_modelInfos.at(i).to_json());
				}
				catch (const errThrow &e)
				{
					throw errThrow::create(1, string("S_modelInfos config to json failed!") + e.what() + "\n");
				}
			}

			return j;
		}

		json_J to_json_Impl_1(void) const
		{
			json_J j;
			try
			{
				j["S_sinksMulti"] = S_sinksMulti.to_json();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("S_sinksMulti config to json failed!") + e.what() + "\n");
			}


			try
			{
				j["S_sinksInfos"] = S_sinksVec.to_json();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("S_sinksInfos config to json failed!") + e.what() + "\n");
			}

			try
			{
				j["S_modelInfos"] = S_modelVec.to_json();
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(1, string("S_modelInfos config to json failed!") + e.what() + "\n");
			}

			return j;
		}

		void from_json_Impl(const json_J &i)
		{
			if (i.find("S_sinksMulti") == i.end())
			{
				throw errThrow::create(1, "loggerCfgInfos config don't has a element: sinksRecCfgInfos\n");
			}

			if (i.find("S_sinksInfos") == i.end())
			{
				errThrow::create(1, "loggerCfgInfos config don't has a element: sinksInfo\n");
			}

			if (i.find("S_modelInfos") == i.end())
			{
				errThrow::create(1, "loggerCfgInfos config don't has a element: modelInfo\n");
			}

			try
			{
				size_t count = 0;
				S_sinksMulti.from_json(i.at("S_sinksMulti"));

				count = i.at("S_sinksInfos").size();
				S_sinksInfos.resize(count);
				for (size_t j = 0; j < count; j++)
				{
					S_sinksInfos[j].from_json(i.at("S_sinksInfos").at(j));
				}
				count = i.at("S_modelInfos").size();
				S_modelInfos.resize(count);
				for (size_t j = 0; j < count; j++)
				{
					S_modelInfos[j].from_json(i.at("S_modelInfos").at(j));
				}
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}

			return;
		}

		void from_json_Impl_1(const json_J &i)
		{
			if (i.find("S_sinksMulti") == i.end())
			{
				throw errThrow::create(1, "loggerCfgInfos config don't has a element: sinksRecCfgInfos\n");
			}

			if (i.find("S_sinksInfos") == i.end())
			{
				errThrow::create(1, "loggerCfgInfos config don't has a element: sinksInfo\n");
			}

			if (i.find("S_modelInfos") == i.end())
			{
				errThrow::create(1, "loggerCfgInfos config don't has a element: modelInfo\n");
			}

			try
			{
				size_t count = 0;
				S_sinksMulti.from_json(i.at("S_sinksMulti"));
				S_sinksVec.from_json(i["S_sinksInfos"]);
				S_modelVec.from_json(i["S_modelInfos"]);
				count = i.at("S_sinksInfos").size();
				S_sinksInfos.resize(count);
				for (size_t j = 0; j < count; j++)
				{
					S_sinksInfos[j].from_json(i.at("S_sinksInfos").at(j));
				}
				count = i.at("S_modelInfos").size();
				S_modelInfos.resize(count);
				for (size_t j = 0; j < count; j++)
				{
					S_modelInfos[j].from_json(i.at("S_modelInfos").at(j));
				}
			}
			catch (const errThrow &e)
			{
				throw errThrow::create(2, string(e.what()) + "\n");
			}

			return;
		}

	public:
		sinksRecCfgInfos S_sinksMulti;
		vector<sinksCfgInfos> S_sinksInfos;
		vector<modelsCfgInfos> S_modelInfos;
	private:
		sinksCfgInfosVec S_sinksVec;
		modelsCfgInfosVec S_modelVec;

	};

	GL4A_INLINE void createDir(const char *str)
	{
		char* pszDir = NULL;
		size_t iLen = 0;
		size_t iRet = 0;

		if (NULL == str)
		{
			throw errThrow::create(1, "Directory is empty!\n");
		}
		else
		{
			pszDir = _strdup(str);
			iLen = strlen(pszDir);
		}
		// �����м�Ŀ¼  
		for (size_t i = 0; i < iLen; i++)
		{
			if (pszDir[i] == '\\' || pszDir[i] == '/')
			{
				pszDir[i] = '\0';
				//���������,����  
				iRet = _access(pszDir, 0);
				if (iRet != 0)
				{
					iRet = _mkdir(pszDir);
					if (iRet != 0)
					{
						throw errThrow::create(2, "Create directory failed!\n");
					}
				}
				//֧��linux,������\����/  
				pszDir[i] = '/';
			}
		}
		if (NULL != pszDir)
		{
			free(pszDir);
			pszDir = NULL;
		}
	}

	GL4A_INLINE void saveCfgFile(const char* V_file, const json_J &V_json)
	{
		try
		{
			createDir(V_file);
			fstream fp(V_file, ios_base::out);
			if (!fp)
			{
				throw errThrow::create(1, "Open file failed!\n");
			}
			else
			{
				fp << V_json.dump(4);
				fp.close();
			}
		}
		catch (const errThrow& e)
		{
			throw errThrow::create(1, string("Open file failed!") + e.what() + "\n");
		}
	}

	GL4A_INLINE void saveCfgFile(const char* V_file, const TotalCfgInfos &S_cfg)
	{
		try
		{
			createDir(V_file);
			fstream fp(V_file, ios_base::out);
			if (!fp)
			{
				throw errThrow::create(1, "Open file failed!\n");
			}
			else
			{
				fp << S_cfg.to_json().dump(4);
				fp.close();
			}
		}
		catch (const errThrow& e)
		{
			throw errThrow::create(1, string("Open file failed!") + e.what() + "\n");
		}
	}

	GL4A_INLINE void loadCfgFile(const char* V_file, json_J &V_json)
	{
		fstream	fp(V_file, ios_base::in);

		if (!fp)
		{
			throw errThrow::create(1, "Open file failed!\n");
		}
		else
		{
			fp >> V_json;
			fp.close();
		}
	}

	GL4A_INLINE json_J loadCfgFile(const char* V_file)
	{
		fstream	fp(V_file, ios_base::in);
		json_J V_json;
		if (!fp)
		{
			throw errThrow::create(1, "Open file failed!\n");
		}
		else
		{
			fp >> V_json;
			fp.close();
		}

		return V_json;
	}

	GL4A_INLINE TotalCfgInfos loadCfgFileStruct(const char* V_file)
	{
		TotalCfgInfos result;
		try
		{
			json_J j = loadCfgFile(V_file);
			result.from_json(j);
		}
		catch (const errThrow &e)
		{
			throw errThrow::create(2, string("Load configure to struct failed!") + e.what() + "\n");
		}
		return result;
	}

}
#endif // !__GL4A_CONFIG_H__







