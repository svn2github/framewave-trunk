/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <cassert>
#include <sstream>
#include <stdarg.h>


// Operating System
#if defined( WIN64   ) || defined( _WIN64   ) || \
    defined( WIN32   ) || defined( _WIN32   ) || \
    defined( WINDOWS ) || defined( _WINDOWS )

#define HC_WIN

#elif defined( LIN64   ) || defined( _LIN64 ) || \
      defined( LIN32   ) || defined( _LIN32 ) || \
      defined( LINUX   ) || defined( _LINUX )

#define HC_LIN
#define sprintf_s sprintf

#elif defined( SOL64   ) || defined( _SOL64   ) || \
      defined( SOL32   ) || defined( _SOL32   ) || \
      defined( SOLARIS ) || defined( _SOLARIS )

#define HC_SOL
#define sprintf_s sprintf

#elif defined( MAC64     ) || defined( _MAC64   ) || \
      defined( MAC32     ) || defined( _MAC32   ) || \
      defined( __APPLE__ )

#define HC_MAC
#define sprintf_s sprintf

#endif // Operating System

#if defined( HC_WIN )
#define HC_CDECL __cdecl
#else
#define HC_CDECL
#endif


namespace
{
	using namespace std;

    bool g_all_headers_mode = false;

	const string::size_type g_min_match_count = 5;		// function name min matching characters
	const float g_min_match_ratio = 0.50f;				// function name matching ratio
	const string g_default_str = "ToDo";				// default string
	const string g_new_line = "\n";						// new line
	const char g_desired_space = ' ';					// blank space

	// structure to store name and type of function parameter
	struct StringParameter
	{
		string m_name;		// name
		string m_type;		// type
	};

	// class to store function declarations
	class FwDocFunction
	{
	private:
		string m_name;									// name of function
		string m_return_type;							// return type of function
		int m_num_arguments;							// number of arguments
		vector<StringParameter> m_parameters;			// parameters and their types

	public:
		// set functions
		void Name(const string& name) { m_name = name; }
		void ReturnType(const string& return_type) { m_return_type = return_type; }
		void NumArguments(const int& num_arguments) { m_num_arguments = num_arguments; }
		void AddParameter(const StringParameter& parameter) { m_parameters.push_back(parameter); }

		// get functions
		const string& Name() const { return m_name; }
		const string& ReturnType() const { return m_return_type; }
		const int& NumArguments() const { return m_num_arguments; }
		const vector<StringParameter>& Parameters() const { return m_parameters; }
	};

    struct FnFeatures
    {
	    unsigned int threaded:1;
	    unsigned int cp_sse2:1;
	    unsigned int cp_sse3:1;
	    unsigned int cp_gh:1;
	    unsigned int cp_future:4;
    };

    struct FunctionIndexEntry
    {
	string name;
        string tag;
        string chapter_name;
        string chapter_tag;
        string section_name;
        string section_tag;
        FnFeatures fnFeatures;

        bool operator<(const FunctionIndexEntry &b) const
        {
            return (name < b.name);
        }
    };

    vector<FunctionIndexEntry> g_fn_index; // global index map    

	class FwDocFunctionBlock
	{
	private:
		string m_name;					// name
		string m_short_description;		// short description
		string m_long_description;		// long description

	public:

        FnFeatures fnFeatures; //todo: make this private member

		FwDocFunctionBlock()
		{
			m_name = g_default_str;
			m_short_description = g_default_str;
			m_long_description = g_default_str;

            fnFeatures.threaded = 0;
            fnFeatures.cp_sse2 = 0;
            fnFeatures.cp_sse3 = 0;
            fnFeatures.cp_gh = 0;
		}

		// set functions
		void Name(const string& name) { m_name = name; }
		void ShortDescription(const string& short_description) { m_short_description = short_description; }
		void LongDescription(const string& long_description) { m_long_description = long_description; }

		void ShortDescriptionAppend(const string& short_description)
		{
			m_short_description += g_new_line;
			m_short_description += short_description;
		}

		void LongDescriptionAppend(const string& long_description)
		{
			m_long_description += g_new_line;
			m_long_description += long_description;
		}

		void Reset()
		{
			m_name = g_default_str;
			m_short_description = g_default_str;
			m_long_description = g_default_str;
		}

		// get functions
		const string& Name() const { return m_name; }
		const string& ShortDescription() const { return m_short_description; }
		const string& LongDescription() const { return m_long_description; }
	};

	// type of Fw documentation parameter
	enum FwDocType
	{
		T_PARAMETER,
		T_RETURN_VALUE,
		T_DATA_TYPE,
		T_DATA_STRUCTURE,
		T_ENUMERATOR,
		T_DEFAULT
	};

	// different types of xml tags
	enum FwDocTagType
	{
		TAG_PARAMETER,
		TAG_RETURN_VALUE,
		TAG_DATA_TYPE,
		TAG_DATA_STRUCTURE,
		TAG_ENUMERATOR,
		TAG_FUNCTION,
		TAG_DEFAULT
	};

	// class for storing tagged names
	class FwPrintTagParameter
	{
	private:
		string m_name;			// name
		FwDocTagType m_type;	// tag type

	public:
		FwPrintTagParameter() {}

		FwPrintTagParameter(
			const string& name,
			const FwDocTagType& type)
		{
			m_name = name;
			m_type = type;
		}

		FwPrintTagParameter& operator=(const FwPrintTagParameter& parameter) 
		{
			m_name = parameter.Name();
			m_type = parameter.Type();
			return *this;
		}

		const string& Name() const { return m_name; }
		const FwDocTagType& Type() const { return m_type; }
	};

	// class to store any Fw documentation parameter,
	// such as function arguments, return values etc.
	class FwDocParameter
	{
	private:
		string m_name;				// name
		string m_description;		// description data
		FwDocType m_type;			// type
		bool m_current;				// is current? use with current function block 
				
	public:
		FwDocParameter() {}

		FwDocParameter(
			const string& name,
			const string& description,
			const FwDocType& type)

		{
			m_name = name;
			m_description = description;
			m_type = type;
			m_current = false;
		}

		FwDocParameter& operator=(const FwDocParameter& parameter)
		{
			m_name = parameter.Name();
			m_description = parameter.Description();
			m_type = parameter.Type();
			m_current = parameter.Current();
			return *this;
		}

		~FwDocParameter() {}

		void DescriptionAppend(const string& description)
		{ 
			m_description += g_new_line;
			m_description += description;
		}

		void MakeCurrent() { m_current = true; }
		void MakeOld() { m_current = false; }

		const string& Name() const { return m_name; }
		const string& Description() const { return m_description; }
		const FwDocType& Type() const { return m_type; }
		const bool& Current() const { return m_current; }
	};

	// removes leading and trailing white spaces
	// replaces all intermediate white spaces by single desired space
	// in place
	void FormatString1(string& buffer)
	{
		if(buffer.length() == 0) return;
		string delimiters = " \r\n\t"; // delimiter symbol array
				
		// trim leading white space
		string::size_type  lead_non_white = buffer.find_first_not_of(delimiters);
		if(lead_non_white == string::npos)
		{
			buffer.resize(0);
			buffer += g_desired_space;
			return;
		}
		buffer.erase(0, lead_non_white);

		// trim trailing white space
		string::size_type trail_non_white = buffer.find_last_not_of(delimiters);
		if(trail_non_white < (buffer.length()-1)) 
			buffer.erase(trail_non_white+1);

		string buffer_copy;

        bool always_true = true;

		while(always_true == true)
		{
			string::size_type  lead_white = buffer.find_first_of(delimiters);
			if(lead_white == string::npos)
			{
				buffer_copy += buffer;
				break;
			}
			else
			{
				buffer_copy += buffer.substr(0, lead_white);
				buffer_copy += g_desired_space;
				buffer.erase(0, lead_white);

				string::size_type  lead_non_white = buffer.find_first_not_of(delimiters);
				buffer.erase(0, lead_non_white);
			}
		}
	
		buffer = buffer_copy;
	}

    string FormatString1_ret(const string &buffer)
    {
        string buffer_copy = buffer;
        FormatString1(buffer_copy);
        return buffer_copy;
    }

	// gets only the first word in a string array
	// in place
	void FormatString2(string& buffer)
	{
		if(buffer.length() == 0) return;
		string delimiters = " \r\n\t"; // delimiter symbol array

		// trim leading white space
		string::size_type lead_non_white = buffer.find_first_not_of(delimiters);
		if(lead_non_white == string::npos)
			return;
		else
			buffer.erase(0, lead_non_white);

		string::size_type  lead_white = buffer.find_first_of(delimiters);
		if(lead_white == string::npos)
			return;
		else
			buffer = buffer.substr(0, lead_white);
	}

	// format the string for printing special characters
	// do not call this after calling TaggedString()
	string FormatString3(const string& buffer)
	{
		string buffer_copy;
		for(string::size_type i=0; i<buffer.length(); i++)
		{
			if(buffer[i] == '<')
				buffer_copy += "&lt;";
			else if(buffer[i] == '>')
				buffer_copy += "&gt;";
			else if(buffer[i] == '&')
				buffer_copy += "&amp;";
			else
				buffer_copy += buffer[i];
		}

		return buffer_copy;
	}

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // FormatString5 and FormatString3 should not be used in unison
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // format the string for printing special characters
	// do not call this after calling TaggedString()
	string FormatString5(const string& buffer)
	{
		string buffer_copy;
        if(buffer.length() == 0)
        {
            buffer_copy = buffer;
            return buffer_copy;
        }

        string::size_type i=0;
        while(i<buffer.length())
		{
			if(buffer[i] == '<')
            {
                string tmp = "";
                bool it_is_a_tag = false;
                unsigned int j=0;
                while(j < 100) // 100 - threshold
                {
                    j++;
                    i++;
                    if(!(i<buffer.length()))
                        break;

                    tmp += buffer[i];

                    if((('a' <= buffer[i]) && ('z' >= buffer[i])) || (('A' <= buffer[i]) && ('Z' >= buffer[i])))
                        continue;
                    else if('>' == buffer[i])
                    {
                        it_is_a_tag = true;
                        break;
                    }
                    else if((j == 1) && ('/' == buffer[i]))
                        continue;
                    else
                        break;
                }

                if(it_is_a_tag)
                    buffer_copy += "<";
                else
                    buffer_copy += "&lt;";

                buffer_copy += tmp;
            }
			else if(buffer[i] == '>')
				buffer_copy += "&gt;";
			else if(buffer[i] == '&')
				buffer_copy += "&amp;";
			else
				buffer_copy += buffer[i];

            i++;
		}

		return buffer_copy;
	}

    // Format only part of a string, assumes contains atleast one of start_tag or end_tag
    string FormatString4(const string& buffer, const string& start_tag, const string& end_tag, bool preserve_tags = true)
    {
        string buffer_t = buffer;
        string accumulate;
        
        bool out_f = false;
        while((buffer_t.find(start_tag) != string::npos) || (buffer_t.find(end_tag) != string::npos))
        {
            if(buffer_t.find(start_tag) != string::npos)
            {
                if(buffer_t.find(end_tag) != string::npos)
                {
                    if(buffer_t.find(start_tag) < buffer_t.find(end_tag))
                    {
                        string tmp = buffer_t.substr(0, buffer_t.find(start_tag));
                        FormatString1(tmp);
                        accumulate += tmp;
                        if(preserve_tags) accumulate += start_tag;
                        buffer_t.erase(0, buffer_t.find(start_tag) + start_tag.size());

                        accumulate += buffer_t.substr(0, buffer_t.find(end_tag));
                        if(preserve_tags) accumulate += end_tag;
                        buffer_t.erase(0, buffer_t.find(end_tag) + end_tag.size());
                        out_f = true;
                    }
                    else
                    {
                        accumulate += buffer_t.substr(0, buffer_t.find(end_tag));
                        if(preserve_tags) accumulate += end_tag;
                        buffer_t.erase(0, buffer_t.find(end_tag) + end_tag.size());

                        string tmp = buffer_t.substr(0, buffer_t.find(start_tag));
                        FormatString1(tmp);
                        accumulate += tmp;
                        if(preserve_tags) accumulate += start_tag;
                        buffer_t.erase(0, buffer_t.find(start_tag) + start_tag.size());
                        out_f = false;
                    }
                }
                else
                {
                        string tmp = buffer_t.substr(0, buffer_t.find(start_tag));
                        FormatString1(tmp);
                        accumulate += tmp;
                        if(preserve_tags) accumulate += start_tag;
                        buffer_t.erase(0, buffer_t.find(start_tag) + start_tag.size());
                        accumulate += buffer_t;
                        buffer_t.erase();
                }
            }
            else
            {
                    accumulate += buffer_t.substr(0, buffer_t.find(end_tag));
                    if(preserve_tags) accumulate += end_tag;
                    buffer_t.erase(0, buffer_t.find(end_tag) + end_tag.size());
                    string tmp = buffer_t;
                    FormatString1(tmp);
                    accumulate += tmp;
                    buffer_t.erase();
            }
        }

        if(out_f)
            FormatString1(buffer_t);

        accumulate += buffer_t;
        return accumulate;
    }

	void CheckFile(bool open, const char *name)
	{
		if(!open)
		{
			printf("\n");
			printf("%s",name);
			printf(" : File could not be opened, exiting..");
			exit(1);
		}
	}

    enum FwHrLevel
    {
        AHL_SECTION,
        AHL_CHAPTER,
        AHL_FUNCTION
    };

    // tag generator
    string GenerateTag(FwHrLevel ahl)
    {
        // counter for tags
        // assumption - sequential access of the heirarchy !!!!!!!!!!!!!!!!!
        static unsigned int sg_section_i = 0;
        static unsigned int sg_chapter_i = 0;
        static unsigned int sg_function_i = 0;

        char tmp[100];
        string tag = "fw_";
        switch(ahl)
        {
        case AHL_SECTION:   sg_chapter_i = 0; sg_function_i = 0; sg_section_i += 10; tag += "section_"; sprintf_s(tmp, "%03d", sg_section_i); break;
        case AHL_CHAPTER:   sg_function_i = 0; sg_chapter_i += 10; tag += "chapter_"; sprintf_s(tmp, "%03d_%04d", sg_section_i, sg_chapter_i); break;
        case AHL_FUNCTION:  sg_function_i += 10; tag += "function_"; sprintf_s(tmp, "%03d_%04d_%05d", sg_section_i, sg_chapter_i, sg_function_i); break;
        default:            printf("SHOULD NOT BE HERE"); exit(1);
        };

        tag += string(tmp);
        return tag;
    }

	// main class
	class HeaderParse
	{
	private:
		ifstream m_in_file;									// input header file object
		ofstream m_xml_file;								// output XML file
		ofstream m_def_file;								// output def file
		string m_buffer;									// string buffer
		char m_c;											// current char
		int m_braces_count;									// embedded braces count
		vector<string> m_names;								// formatted names
		string m_delimiters;								// delimiters to be defined on the fly
		map<string, FwDocParameter> m_fw_records;			// map for storing all fw documentation parameters
		vector<FwDocFunction> m_fw_functions;				// vector for storing all function declarations
		FwDocFunctionBlock m_function_block;				// current funtion block
		map<string, FwPrintTagParameter> m_tag_records;	// map for storing xml tags for different names
		bool m_do_doc;										// flag to indicate active documentation status
		map<FwDocTagType, string> m_prefix_tags;			// prefix xml tags
		map<FwDocTagType, string> m_suffix_tags;			// suffix xml tags
		int m_line_num;										// line number for testing
		bool m_log_print;									// generate log?
		bool m_pound_inactive;								// inside pound blocks for FW_BUILD_NUM
		bool m_auto_function_block;							// generate auto function block?

		bool m_ipp;											// add ipp aliases in def file
		string m_ipp_symbol_file_name;						// ipp symbol file location
		string m_def_file_name;								// def file name stored for write-read-write
		string m_xml_file_name;								// xml file name stored for write-read-write
		string m_in_file_name;								// in file name

		ifstream m_pch_file;								// input buildnum file
		ofstream m_out_file;								// output header file
		unsigned int m_version_num;							// version number
		string m_out_buffer;
        string m_section_name;                              // section name
        string m_section_tag;                               // section tag
        string m_current_chapter_name;                      // current chapter name
        string m_current_chapter_tag;                       // current chapter tag

        enum
		{
			S_BASE,
			
			S_SLASH,
			S_LINE_COMMENT,
			S_BLOCK_COMMENT,
			S_BLOCK_COMMENT_STAR,
			S_POUND_DIRECTIVE,

			S_FUNCTION,
			S_FW_BLOCK

		} m_state;		// parsing state

		// functions act based on current character from input file
		// and the current parsing state
		void Text();
		void Slash();
		void Star();
		void Pound();
		void EndOfLine();

		// functions to parse through nested flower braces
		void Braces();
		bool BracesEnd();

		//init routines
		void Init();
		void InitXml();
		void InitDef(const char* def_dll_name);

		// main functions for documentation system
		void FwKeywordBlock();
		void DocFunctionBlock();
		void DocDataBlock();
		void DocDocumentationBlock();
		void DocFunctions();
		bool IsFunctionRight(const string& str, const string& substr);
		bool IsFunctionSimilar(const string& str1, const string& str2);
		void AutoFunctionBlock(const string& str);

		void ParsePch();
		void PoundInactive();
		void OutFileComment(const string& buffer);

		// functions that support printing out data
		void TaggedString();
		void PrintDataTypes(bool current_only = true);
		void PrintDataStructures(bool current_only = true);
		void PrintEnumerators(bool current_only = true);
		void PrintParameters(bool current_only = true);
		void PrintReturnValues(bool current_only = true);
		void PrintFunctions();
		void PrintFunctionHead();
		void PrintFunctionTail();
		void PrintFunctionFeatures();
        void PrintFunctionPrfx();
        void PrintFunctionSufx();
		void PrintAll(bool def_last = false);
		void PrintUnDescribed();

		// add-on functions
		void IppFwDef();
		void SplitXmlFile();

		// string manipulation functions
		void TrimString();
		void GetTwoStringsType1();
		void GetTwoStringsType2();
		void GetTwoStringsType3();
		
	public:
		HeaderParse(
			const char* in_file_name,
			const char* pch_file_name,
			const char* out_file_name,
			const char* xml_file_name,
			const char* def_file_name,
			const char* def_dll_name);

		HeaderParse(
			const char* in_file_name,
			const char* pch_file_name,
			const char* out_file_name,
			const char* xml_file_name);

		HeaderParse(
			const char* in_file_name,
			const char* pch_file_name,
			const char* out_file_name,
			const char* def_file_name,
			const char* def_dll_name);

		~HeaderParse();
	
		void Parse();
	};

	void HeaderParse::Init()
	{
		// create the tags
		m_prefix_tags[TAG_PARAMETER] = "<ParameterName>";
		m_suffix_tags[TAG_PARAMETER] = "</ParameterName>";
		m_prefix_tags[TAG_RETURN_VALUE] = "<ReturnValue>";
		m_suffix_tags[TAG_RETURN_VALUE] = "</ReturnValue>";
		m_prefix_tags[TAG_DATA_TYPE] = "<Type>";
		m_suffix_tags[TAG_DATA_TYPE] = "</Type>";
		m_prefix_tags[TAG_DATA_STRUCTURE] = "<Type>";
		m_suffix_tags[TAG_DATA_STRUCTURE] = "</Type>";
		m_prefix_tags[TAG_ENUMERATOR] = "<Type>";
		m_suffix_tags[TAG_ENUMERATOR] = "</Type>";
		m_prefix_tags[TAG_FUNCTION] = "<FunctionName>";
		m_suffix_tags[TAG_FUNCTION] = "</FunctionName>";
		m_prefix_tags[TAG_DEFAULT] = "<DefaultTag>";
		m_suffix_tags[TAG_DEFAULT] = "</DefaultTag>";
	}

	void HeaderParse::InitXml()
	{
		// header for xml file
		m_xml_file << "\n";
		m_xml_file << "<Section>\n";
		m_xml_file << "\n";
	}

	void HeaderParse::InitDef(const char* def_dll_name)
	{
		// header for def file
		m_def_file << "\nLIBRARY\t" << def_dll_name << g_new_line;
		m_def_file << "\nEXPORTS\n";
	}

	HeaderParse::HeaderParse(
		const char* in_file_name,
		const char* pch_file_name,
		const char* out_file_name,
		const char* xml_file_name) :	m_in_file(in_file_name), m_pch_file(pch_file_name), m_out_file(out_file_name), 
										m_xml_file(xml_file_name), m_state(S_BASE), m_braces_count(0), m_do_doc(false),
										m_line_num(0), m_log_print(true), m_pound_inactive(false), m_ipp(false),
										m_xml_file_name(xml_file_name), m_in_file_name(in_file_name), m_auto_function_block(true)
	{
		CheckFile(m_in_file.is_open(), in_file_name);
		CheckFile(m_pch_file.is_open(), pch_file_name);
		CheckFile(m_out_file.is_open(), out_file_name);
		CheckFile(m_xml_file.is_open(), xml_file_name);

		Init();
		InitXml();
	}

	HeaderParse::HeaderParse(
		const char* in_file_name,
		const char* pch_file_name,
		const char* out_file_name,
		const char* def_file_name,
		const char* def_dll_name) :		m_in_file(in_file_name), m_pch_file(pch_file_name), m_out_file(out_file_name),
										m_def_file(def_file_name), m_state(S_BASE), m_braces_count(0), m_do_doc(false),
										m_line_num(0), m_log_print(false), m_pound_inactive(false), m_ipp(false),
										m_def_file_name(def_file_name), m_in_file_name(in_file_name), m_auto_function_block(true)
	{
		CheckFile(m_in_file.is_open(), in_file_name);
		CheckFile(m_pch_file.is_open(), pch_file_name);
		CheckFile(m_out_file.is_open(), out_file_name);
		CheckFile(m_def_file.is_open(), def_file_name);

		Init();
		InitDef(def_dll_name);
	}

	HeaderParse::HeaderParse(
		const char* in_file_name,
		const char* pch_file_name,
		const char* out_file_name,
		const char* xml_file_name,
		const char* def_file_name,
		const char* def_dll_name) :		m_in_file(in_file_name), m_pch_file(pch_file_name), m_out_file(out_file_name),
										m_xml_file(xml_file_name), m_def_file(def_file_name), m_state(S_BASE),
										m_braces_count(0), m_do_doc(false), m_line_num(0), m_log_print(true),
										m_pound_inactive(false), m_ipp(false), m_def_file_name(def_file_name),
										m_xml_file_name(xml_file_name), m_in_file_name(in_file_name), m_auto_function_block(true)
	{
		CheckFile(m_in_file.is_open(), in_file_name);
		CheckFile(m_pch_file.is_open(), pch_file_name);
		CheckFile(m_out_file.is_open(), out_file_name);
		CheckFile(m_xml_file.is_open(), xml_file_name);
		CheckFile(m_def_file.is_open(), def_file_name);

		Init();
		InitXml();
		InitDef(def_dll_name);
	}

	HeaderParse::~HeaderParse()
	{
		if(m_in_file.eof())
			m_in_file.close();
		else
		{
			printf("Parsing cannot be stopped before EOF, exiting..");
			exit(1);
		}

		if((m_xml_file && m_xml_file.is_open()) || (m_def_file && m_def_file.is_open()))
			PrintAll(true); // print once at the end

		if(m_xml_file && m_xml_file.is_open())
		{
			// footnote
			m_xml_file << "\n";
			m_xml_file << "</Section>\n";
			PrintUnDescribed();
			m_xml_file.close();

            if(!g_all_headers_mode)
			    SplitXmlFile();
		}
		if(m_def_file && m_def_file.is_open())
		{
			m_def_file.close();
			IppFwDef();
		}

		m_pch_file.close();

		m_out_file << m_out_buffer;
		m_out_file.close();
	}

	// keeps track of nested braces and ignores content
	// for use with enum, class, struct etc.. that use { and }
	void HeaderParse::Braces()
	{
		if(BracesEnd())
			m_braces_count--;
		else
			m_braces_count++;

		if(m_braces_count == 0)
			return;
		else
			Braces();
	}
	
	bool HeaderParse::BracesEnd()
	{
		char tmp = 'a'; // some initial value
		while(!m_in_file.eof())
		{
			m_c = (char) m_in_file.get();
			m_out_buffer += m_c;
			if(m_c == '\n')
				m_line_num++;

			if(tmp == '/' && m_c == '/') // line comment
			{
				char c = 'a';
				while(c != '\n') { c = (char)m_in_file.get(); m_out_buffer += c; }
				m_line_num++;
				tmp = 'a';
				continue;
			}

			if(tmp == '/' && m_c == '*') // block comment
			{
				do
				{
					tmp = m_c;
					m_c = (char)m_in_file.get();
					m_out_buffer += m_c;
					if(m_c == '\n')
						m_line_num++;
				}while(!(tmp== '*' && m_c == '/'));
				tmp = 'a';
				continue;
			}

			if(m_c == '}')
				return true;
			else if(m_c == '{')
				return false;

			tmp = m_c;
		}

		printf("(%d)[Invalid use of braces '{' and '}']\nCheck braces in class, enum, struct etc.. Parsing stopped.",(m_line_num+1));
		printf("\n");
		exit(1);
		//return true;
	}

	void HeaderParse::TrimString()
	{
		const char* delimiters = m_delimiters.c_str(); // delimiter symbol array

		string::size_type  lead_non_white = m_buffer.find_first_not_of(delimiters);
		m_buffer.erase(0, lead_non_white);

		string::size_type trail_non_white = m_buffer.find_last_not_of(delimiters);
		m_buffer.erase(trail_non_white+1);
	}

	// first part and second part in that order
	void HeaderParse::GetTwoStringsType1()
	{
		assert(m_names.size() == 0); // m_names has to be empty
		m_names.resize(0); // for release mode

		const char* delimiters = m_delimiters.c_str(); // delimiter symbol array
		
		// get the first part intially
		string::size_type  lead_non_white = m_buffer.find_first_not_of(delimiters);
		m_buffer.erase(0, lead_non_white);

		string::size_type first_white = m_buffer.find_first_of(delimiters);
		m_names.push_back(m_buffer.substr(0, first_white));
		m_buffer.erase(0, first_white);

		// get the second part finally
		lead_non_white = m_buffer.find_first_not_of(delimiters);
		m_buffer.erase(0, lead_non_white);

		first_white = m_buffer.find_first_of(delimiters);
		m_names.push_back(m_buffer.substr(0, first_white));
		m_buffer.resize(0); // m_buffer is flushed
	}

	// second part and first part in that order
	void HeaderParse::GetTwoStringsType2()
	{
		assert(m_names.size() == 0); // m_names has to be empty
		m_names.resize(0); // for release mode

		const char* delimiters = m_delimiters.c_str(); // delimiter symbol array

		// get the second part initially
		string::size_type trail_non_white = m_buffer.find_last_not_of(delimiters);
		m_buffer.erase(trail_non_white+1);

		string::size_type  trail_last_white = m_buffer.find_last_of(delimiters);
		string tmp = m_buffer.substr(trail_last_white+1);
		m_buffer.erase(trail_last_white+1);

		// get the first part finally
		string::size_type lead_non_white = m_buffer.find_first_not_of(delimiters);
		m_buffer.erase(0, lead_non_white);
		m_names.push_back(m_buffer);
		m_names.push_back(tmp);
		m_buffer.resize(0); // m_buffer is flushed
	}

	// first part and the remaining part in that order
	void HeaderParse::GetTwoStringsType3()
	{
		assert(m_names.size() == 0); // m_names has to be empty
		m_names.resize(0); // for release mode

		const char* delimiters = m_delimiters.c_str(); // delimiter symbol array
		
		// get the first part intially
		string::size_type  lead_non_white = m_buffer.find_first_not_of(delimiters);
		m_buffer.erase(0, lead_non_white);

		string::size_type first_white = m_buffer.find_first_of(delimiters);
		m_names.push_back(m_buffer.substr(0, first_white));
		m_buffer.erase(0, first_white);

		// get the remaining part finally
		lead_non_white = m_buffer.find_first_not_of(delimiters);
		m_buffer.erase(0, lead_non_white);

		m_names.push_back(m_buffer);
		m_buffer.resize(0); // m_buffer is flushed
	}

	// check to see if function block name and functions under are consistent
	bool HeaderParse::IsFunctionRight(const string& str, const string& substr)
	{
		string str_copy;
		for(string::size_type i=0; i<str.size(); i++)
			if(str[i] >= 'A' && str[i] <= 'Z') // convert to lower case
				str_copy += (str[i] + 'a' - 'A');
			else
				str_copy += str[i];

		string substr_copy;
		for(string::size_type i=0; i<substr.size(); i++)
			if(substr[i] >= 'A' && substr[i] <= 'Z')	// convert to lower case
				substr_copy += (substr[i] + 'a' - 'A');
			else
				substr_copy += substr[i];

		if(str_copy.find(substr_copy) != string::npos)
			return true;
		else
			return false;
	}

	// check to see if function block name and functions under are similar
	bool HeaderParse::IsFunctionSimilar(const string& str1, const string& str2)
	{
		string str_a, str_b;
		if(str2.length() > str1.length())
		{
			str_a = str2;
			str_b = str1;
		}
		else
		{
			str_a = str1;
			str_b = str2;
		}

		for(string::size_type i=0; i<str_a.length(); i++)
			if(str_a[i] >= '0' && str_a[i] <= '9')
				str_a[i] = '_';

		for(string::size_type i=0; i<str_b.length(); i++)
			if(str_b[i] >= '0' && str_b[i] <= '9')
				str_b[i] = '_';

		string::size_type length = str_b.length();
		string::size_type i = 0;
        bool always_true = true;
		while(always_true == true)
		{
			if(str_b.length() == 0)
				break;

			if(str_a.find(str_b) != string::npos)
				break;
			else
			{
				str_b.erase(str_b.length()-1); // erase last character
				i++;
			}
		}
		string::size_type match_count = length - i;
	
		if((match_count >= g_min_match_count) || ((((float)match_count)/((float)length)) > g_min_match_ratio))
			return true;
		else
			return false;
	}

	// retrieve function block name
	void HeaderParse::AutoFunctionBlock(const string& str)
	{
		m_buffer.resize(0);
		string::size_type fwi_pos = str.find("fw");
		if(fwi_pos == string::npos) return;

		string::size_type clip_num;
		clip_num = ((str[2] >= 'a' && str[2] <= 'z') ? 3 : 2 ); // 4 means length of fw and one more character, fwi or fws etc
		m_buffer = str.substr(fwi_pos+clip_num);
		string::size_type first_underscore = m_buffer.find_first_of('_');
		if(first_underscore != string::npos)
			m_buffer.erase(first_underscore);

		m_function_block.Name(m_buffer);
	}

	// Get tagged string based on entries in m_tag_records
	void HeaderParse::TaggedString()
	{
        string tmp = FormatString5(m_buffer);
        m_buffer = tmp;

		string name_chars_allowed;
		for(unsigned char i='!'; i<='~'; i++)
			name_chars_allowed += i;

		m_delimiters = " \r\n\t"; // dont include <, > or & sign in this
		string buffer = "";

		bool exit_t = false;
        bool always_true = true;
		while(always_true == true)
		{
			string::size_type first_non_white = m_buffer.find_first_of(name_chars_allowed);

			if(first_non_white == string::npos)
			{
				buffer += m_buffer;
				m_buffer.resize(0);
				break;
			}
			else
			{
				buffer += m_buffer.substr(0,first_non_white);
				m_buffer.erase(0,first_non_white);
				string::size_type first_white = m_buffer.find_first_not_of(name_chars_allowed);

				// getting a word from the string array
				string word;
				if(first_white == string::npos)
				{
					word = m_buffer;
					m_buffer.resize(0);
					exit_t = true;
				}
				else
				{
					word = m_buffer.substr(0,first_white);
					m_buffer.erase(0,first_white);
				}

                // disable tagging

				//map<string, FwPrintTagParameter>::const_iterator m_tr_p;
				//m_tr_p = m_tag_records.find(word);
				//if(m_tr_p != m_tag_records.end())
				//{
				//	buffer += m_prefix_tags[(m_tag_records[(m_tr_p->second).Name()]).Type()];
				//	buffer += FormatString3(word);
				//	buffer += m_suffix_tags[(m_tag_records[(m_tr_p->second).Name()]).Type()];
				//}
				//else
				//  buffer += FormatString3(word);

                buffer += word;
			}

			if(exit_t)
				break;
		}
		
		buffer += "";
		m_buffer = buffer;
	}

	void HeaderParse::PrintFunctions()
	{
		m_def_file << g_new_line;

		m_xml_file << "<FunctionDeclarationList>\n"; // start function declarations
		vector<FwDocFunction>::const_iterator m_af_p;
		for(m_af_p = m_fw_functions.begin(); m_af_p != m_fw_functions.end(); m_af_p++)
		{
			m_xml_file << "\t<FunctionDeclaration>\n";
			m_xml_file << "\t\t<ReturnType>" << FormatString1_ret(FormatString3(m_af_p->ReturnType())) << "</ReturnType>\n";
			m_xml_file << "\t\t<FunctionName>" << FormatString1_ret(FormatString3(m_af_p->Name())) << "</FunctionName>\n";

			m_xml_file << "\t\t<ParameterList>\n";
			vector<StringParameter>::const_iterator param_p;
			for(param_p = (m_af_p->Parameters()).begin(); param_p != (m_af_p->Parameters()).end(); param_p++)
			{
				m_xml_file << "\t\t\t<Parameter>\n";
				m_xml_file << "\t\t\t\t<Type>" << FormatString3(param_p->m_type) << "</Type>\n";
				m_xml_file << "\t\t\t\t<ParameterName>" << FormatString3(param_p->m_name) << "</ParameterName>\n";
				m_xml_file << "\t\t\t</Parameter>\n";
			}
			m_xml_file << "\t\t</ParameterList>\n";
			m_xml_file << "\t</FunctionDeclaration>\n";

			m_def_file << m_af_p->Name() << g_new_line;
		}
		m_xml_file << "</FunctionDeclarationList>\n"; // end function declarations
		m_xml_file << "\n";
	}

	void HeaderParse::PrintDataStructures(bool current_only)
	{
		ostringstream tmp_stream;
		bool got_printed = false;

		// printing data types
		tmp_stream << "<DefinitionList>\n"; // start documenting data types
		map<string,FwDocParameter>::iterator m_ar_p;
		for(m_ar_p = m_fw_records.begin(); m_ar_p != m_fw_records.end(); m_ar_p++)
		{
			bool do_print;
			if(current_only)
				do_print = (((m_ar_p->second).Type() == T_DATA_STRUCTURE) && ((m_ar_p->second).Current() == true));
			else
				do_print = ((m_ar_p->second).Type() == T_DATA_STRUCTURE);

			if(do_print)
			{
				got_printed = true;
				tmp_stream << "\t<DefinitionEntry>\n";
				tmp_stream << "\t\t<Term><DataType>\t" << FormatString3((m_ar_p->second).Name()) << "\t</DataType></Term>\n" ;
				tmp_stream << "\t\t<Definition>\t" << FormatString3((m_ar_p->second).Description()) << "\t</Definition>\n" ;
				tmp_stream << "\t</DefinitionEntry>\n";

				if(current_only) (m_ar_p->second).MakeOld();
			}
		}
		tmp_stream << "</DefinitionList>\n"; // end documenting data types
		tmp_stream << "\n";

		if(got_printed)
			m_xml_file << tmp_stream.str();
	}

	void HeaderParse::PrintEnumerators(bool current_only)
	{
		ostringstream tmp_stream;
		bool got_printed = false;

		// printing data types
		tmp_stream << "<DefinitionList>\n"; // start documenting data types
		map<string,FwDocParameter>::iterator m_ar_p;
		for(m_ar_p = m_fw_records.begin(); m_ar_p != m_fw_records.end(); m_ar_p++)
		{
			bool do_print;
			if(current_only)
				do_print = (((m_ar_p->second).Type() == T_ENUMERATOR) && ((m_ar_p->second).Current() == true));
			else
				do_print = ((m_ar_p->second).Type() == T_ENUMERATOR);

			if(do_print)
			{
				got_printed = true;
				tmp_stream << "\t<DefinitionEntry>\n";
				tmp_stream << "\t\t<Term><DataType>\t" << FormatString3((m_ar_p->second).Name()) << "\t</DataType></Term>\n" ;
				tmp_stream << "\t\t<Definition>\t" << FormatString3((m_ar_p->second).Description()) << "\t</Definition>\n" ;
				tmp_stream << "\t</DefinitionEntry>\n";

				if(current_only) (m_ar_p->second).MakeOld();
			}
		}
		tmp_stream << "</DefinitionList>\n"; // end documenting data types
		tmp_stream << "\n";

		if(got_printed)
			m_xml_file << tmp_stream.str();
	}

	void HeaderParse::PrintDataTypes(bool current_only)
	{
		ostringstream tmp_stream;
		bool got_printed = false;

		// printing data types
		tmp_stream << "<DefinitionList>\n"; // start documenting data types
		map<string,FwDocParameter>::iterator m_ar_p;
		for(m_ar_p = m_fw_records.begin(); m_ar_p != m_fw_records.end(); m_ar_p++)
		{
			bool do_print;
			if(current_only)
				do_print = (((m_ar_p->second).Type() == T_DATA_TYPE) && ((m_ar_p->second).Current() == true));
			else
				do_print = ((m_ar_p->second).Type() == T_DATA_TYPE);

			if(do_print)
			{
				got_printed = true;
				tmp_stream << "\t<DefinitionEntry>\n";
				tmp_stream << "\t\t<Term><DataType>\t" << FormatString3((m_ar_p->second).Name()) << "\t</DataType></Term>\n" ;
				tmp_stream << "\t\t<Definition>\t" << FormatString3((m_ar_p->second).Description()) << "\t</Definition>\n" ;
				tmp_stream << "\t</DefinitionEntry>\n";

				if(current_only) (m_ar_p->second).MakeOld();
			}
		}
		tmp_stream << "</DefinitionList>\n"; // end documenting data types
		tmp_stream << "\n";

		if(got_printed)
			m_xml_file << tmp_stream.str();
	}

	void HeaderParse::PrintParameters(bool current_only)
	{
		ostringstream tmp_stream;
		bool got_printed = false;

		if(current_only)
			tmp_stream << "<ParameterDefinitionList>\n";

		// printing parameters
		tmp_stream << "<DefinitionList>\n"; // start documenting parameters
		map<string,FwDocParameter>::iterator m_ar_p;
		for(m_ar_p = m_fw_records.begin(); m_ar_p != m_fw_records.end(); m_ar_p++)
		{
			bool do_print;
			if(current_only)
				do_print = (((m_ar_p->second).Type() == T_PARAMETER) && ((m_ar_p->second).Current() == true));
			else
				do_print = ((m_ar_p->second).Type() == T_PARAMETER);

			if(do_print)
			{
				got_printed = true;
				tmp_stream << "\t<DefinitionEntry>\n";
				tmp_stream << "\t\t<Term><ParameterName>\t" << FormatString3((m_ar_p->second).Name()) << "\t</ParameterName></Term>\n" ;
				tmp_stream << "\t\t<Definition>\t" << FormatString3((m_ar_p->second).Description()) << "\t</Definition>\n" ;
				tmp_stream << "\t</DefinitionEntry>\n";

				if(current_only) (m_ar_p->second).MakeOld();
			}
		}
		tmp_stream << "</DefinitionList>\n"; // end documenting parameters
		tmp_stream << "\n";

		if(current_only)
			tmp_stream << "</ParameterDefinitionList>\n";

		if(got_printed)
			m_xml_file << tmp_stream.str();
	}

	void HeaderParse::PrintReturnValues(bool current_only)
	{
		ostringstream tmp_stream;
		bool got_printed = false;

		if(current_only)
			tmp_stream << "<ReturnedValueList>\n";

		// printing return values
		tmp_stream << "<DefinitionList>\n"; // start documenting return values
		map<string,FwDocParameter>::iterator m_ar_p;
		for(m_ar_p = m_fw_records.begin(); m_ar_p != m_fw_records.end(); m_ar_p++)
		{
			bool do_print;
			if(current_only)
				do_print = (((m_ar_p->second).Type() == T_RETURN_VALUE) && ((m_ar_p->second).Current() == true));
			else
				do_print = ((m_ar_p->second).Type() == T_RETURN_VALUE);

			if(do_print)
			{
				got_printed = true;
				tmp_stream << "\t<DefinitionEntry>\n";
				tmp_stream << "\t\t<Term><Constant>\t" << FormatString3((m_ar_p->second).Name()) << "\t</Constant></Term>\n" ;
				tmp_stream << "\t\t<Definition>\t" << FormatString3((m_ar_p->second).Description()) << "\t</Definition>\n" ;
				tmp_stream << "\t</DefinitionEntry>\n";

				if(current_only) (m_ar_p->second).MakeOld();
			}
		}
		tmp_stream << "</DefinitionList>\n"; // end documenting return values
		tmp_stream << "\n";

		if(current_only)
			tmp_stream << "</ReturnedValueList>\n";

		if(got_printed)
			m_xml_file << tmp_stream.str();
	}

    void HeaderParse::PrintFunctionPrfx()
    {
        m_xml_file << "\n<FunctionBlock>\n";
		m_xml_file << "\n";
    }

    void HeaderParse::PrintFunctionSufx()
    {
        m_xml_file << "</FunctionBlock>\n";
		m_xml_file << "\n";
    }

	void HeaderParse::PrintFunctionHead()
	{
        m_xml_file << "<FunctionHeading>\n";
        string fn_name = FormatString3(m_function_block.Name());
        string fn_tag = GenerateTag(AHL_FUNCTION);
		m_xml_file << "<FunctionName>"; m_xml_file << fn_name; m_xml_file << "</FunctionName>\n";
        m_xml_file << "<FunctionLink>"; m_xml_file << fn_tag; m_xml_file << "</FunctionLink>\n";
		m_xml_file << "</FunctionHeading>\n";
		m_xml_file << "\n";
		m_xml_file << "<ShortDescription>\n";
		m_xml_file << FormatString3(m_function_block.ShortDescription()) << "\n";
		m_xml_file << "</ShortDescription>\n";
		m_xml_file << "\n";

        FunctionIndexEntry entry;
        entry.name = fn_name;
        entry.tag = fn_tag;
        entry.chapter_name = m_current_chapter_name;
        entry.chapter_tag = m_current_chapter_tag;
        entry.section_name = m_section_name;
        entry.section_tag = m_section_tag;
        entry.fnFeatures = m_function_block.fnFeatures;
        g_fn_index.push_back(entry);

	}

	void HeaderParse::PrintFunctionTail()
	{
		m_xml_file << "<DetailedDescription>\n";
		m_xml_file << "<Body>\n";

		m_buffer = m_function_block.LongDescription();
		TaggedString();
		m_xml_file << m_buffer << "\n";
		m_buffer.resize(0);

		m_xml_file << "</Body>\n";
		m_xml_file << "</DetailedDescription>\n";
		m_xml_file << "\n";
	}

	void HeaderParse::PrintFunctionFeatures()
	{
		bool mt_p = false, sse2_p = false, sse3_p = false, gh_p = false;

        if(m_function_block.fnFeatures.threaded)    mt_p   = true;
        if(m_function_block.fnFeatures.cp_sse2)     sse2_p = true;
		if(m_function_block.fnFeatures.cp_sse3)     sse3_p = true;
        if(m_function_block.fnFeatures.cp_gh)       gh_p   = true;

		if( mt_p | sse2_p | sse3_p | gh_p )
		{
			m_xml_file << "\n";
			m_xml_file << "<SupportedTechnologies>\n";
			m_xml_file << "<SupportedTechnologiesDefinition>";

			bool comma = false;
			if(mt_p)   {								m_xml_file << "MT";			comma = true; }
			if(sse2_p) { if(comma) m_xml_file << ", ";	m_xml_file << "SSE2";		comma = true; }
			if(sse3_p) { if(comma) m_xml_file << ", ";	m_xml_file << "SSE3";		comma = true; }
			if(gh_p)   { if(comma) m_xml_file << ", ";	m_xml_file << "Family10h";	comma = true; }
			
			m_xml_file << "</SupportedTechnologiesDefinition>\n";
			m_xml_file << "</SupportedTechnologies>\n";
			m_xml_file << "\n";
		}
	}

	void HeaderParse::PrintAll(bool def_last)
	{
		string default_str = "ToDo";
		if((m_function_block.Name() != default_str) || (def_last && (m_fw_functions.size() > 0)))
		{
            PrintFunctionPrfx();
			PrintFunctionHead();
            PrintFunctionFeatures();
			PrintFunctions();
			PrintParameters();
			PrintFunctionTail();
			PrintReturnValues();
            PrintFunctionSufx();

			m_function_block.Reset();
			m_fw_functions.resize(0);
		}
	}

	// print all the parameters that were undescribed
	void HeaderParse::PrintUnDescribed()
	{
		if(m_log_print)
		{
			bool got_printed = false;
			ostringstream tmp_stream;

			string in_file_name_only = m_in_file_name;
			if(in_file_name_only.find_last_of("/\\") != string::npos)
				in_file_name_only.erase(0, in_file_name_only.find_last_of("/\\")+1);

			tmp_stream << "\n\n"; tmp_stream << in_file_name_only.c_str(); tmp_stream << ": ";
			tmp_stream << "List of undescribed terms:\n\n";

			string default_str = "ToDo";
			map<string,FwDocParameter>::iterator m_ar_p;
			for(m_ar_p = m_fw_records.begin(); m_ar_p != m_fw_records.end(); m_ar_p++)
				if((m_ar_p->second).Description() == default_str)
				{
					got_printed = true;
					string gda = (m_ar_p->second).Name();
					tmp_stream << ((m_ar_p->second).Name()).c_str();
					tmp_stream << "\n";
				}

			if(got_printed)
				cout << tmp_stream.str();
		}
	}

	// document function block
	void HeaderParse::DocFunctionBlock()
	{
        m_function_block.fnFeatures.threaded = 0;
        m_function_block.fnFeatures.cp_sse2 = 0;
        m_function_block.fnFeatures.cp_sse3 = 0;
        m_function_block.fnFeatures.cp_gh = 0;

		char new_line = '\n';
		string buffer_line;
		string current_key;
		bool exit_t = false;
		unsigned int num_line_breaks_long_desc = 0;
        bool pseudo_mode = false;
        bool always_true = true;
		while(always_true == true)
		{
			if(m_buffer.length() == 0)
				break;

			// handle one line at a time
			string::size_type new_line_pos = m_buffer.find_first_of(new_line);
			if(new_line_pos == string::npos)
			{
				buffer_line = m_buffer;
				exit_t = true;
			}
			else
			{
				buffer_line = m_buffer.substr(0, new_line_pos);
				m_buffer.erase(0, new_line_pos+1);
			}

			string buffer_line_unformatted = buffer_line;

			string delimiters = " \n\r\t";
			string::size_type lead_non_white = buffer_line.find_first_not_of(delimiters);
			if(lead_non_white == string::npos)
			{
				if(exit_t)
					break;
				else
				{
					num_line_breaks_long_desc++;
					continue;
				}
			}
			else
				buffer_line.erase(0, lead_non_white);

			string tech_list = "Technologies";
            string short_des = "Short";
			string long_des = "Long";
			string return_values = "ReturnValues";

			//check if first character is #
			char keyword_start = '#'; // token to mark start of keyword
			if(buffer_line[0] == keyword_start)
			{
                FormatString1(buffer_line);
				string keyword = buffer_line;
				FormatString2(keyword);
				keyword.erase(0,1); // erase the pound sign
				buffer_line.erase(0,1);

                string partial_formatted = buffer_line_unformatted;
                if((buffer_line.rfind("<Pseudo>") != string::npos) && (buffer_line.rfind("</Pseudo>") != string::npos))
                {
                    if(buffer_line.rfind("<Pseudo>") > buffer_line.rfind("</Pseudo>"))
                        pseudo_mode = true;
                    else
                        pseudo_mode = false;

                    partial_formatted = FormatString4(buffer_line_unformatted, "<Pseudo>", "</Pseudo>");
                }
                else
                {
                    if(buffer_line.rfind("<Pseudo>") != string::npos)
                    {
                        partial_formatted = FormatString4(buffer_line_unformatted, "<Pseudo>", "</Pseudo>");
                        pseudo_mode = true;
                    }
                    else if(buffer_line.rfind("</Pseudo>") != string::npos)
                    {
                        partial_formatted = FormatString4(buffer_line_unformatted, "<Pseudo>", "</Pseudo>");
                        pseudo_mode = false;
                    }
                    else
                    {
                        if(pseudo_mode)
                            partial_formatted = buffer_line_unformatted;
                        else
                            partial_formatted = buffer_line;
                    }
                }

                if(keyword == tech_list)
                {
					current_key = tech_list;
                    char delimiter_symbol = ','; // delimiter for the separate return values
					if(buffer_line.length() > (keyword.length()+3))
					{
						buffer_line.erase(0, keyword.length()+3);
						string word;
						for(string::size_type i=0; i<buffer_line.length(); i++)
						{
							if((buffer_line[i] == delimiter_symbol) || 
								((i == (buffer_line.length()-1)) && (word.size() > 0)))
							{
								if(i == (buffer_line.length()-1))
									if( buffer_line[i] != ',' )
										word += buffer_line[i];
								FormatString2(word); // get a single tech feature

                                if(word == string("REF")) word;
                                if(word == string("MT")) m_function_block.fnFeatures.threaded = 1;
                                if(word == string("SSE2")) m_function_block.fnFeatures.cp_sse2 = 1;
                                if(word == string("SSE3")) m_function_block.fnFeatures.cp_sse3 = 1;
                                if(word == string("F10H")) m_function_block.fnFeatures.cp_gh = 1;

								word.resize(0);
							}
							else
								word += buffer_line[i];
						}
					}
                }
				else if(keyword == short_des)
				{
					current_key = short_des;
					if(buffer_line.length() > (keyword.length()+3))
					{
						buffer_line.erase(0, keyword.length()+3);
						m_function_block.ShortDescription(buffer_line);
					}
				}
				else if(keyword == long_des)
				{
					current_key = long_des;
					if(buffer_line.length() > (keyword.length()+3))
                    {
                        buffer_line.erase(0, keyword.length()+3);
                        partial_formatted.erase(0, keyword.length()+3);
                        if((partial_formatted.find_first_not_of(" \r\n\t") != string::npos) && (partial_formatted[partial_formatted.find_first_not_of(" \r\n\t")] == '-'))
                        {
                            printf("(%d)[Function block has invalid '-' sign at the beginning]\nCheck function block. Error, Parsing stopped.",(m_line_num+1));
                            exit(1);
                        }

                        m_function_block.LongDescription(partial_formatted);
                    }

					num_line_breaks_long_desc = 0;
				}
				else if(keyword == return_values)
				{
					current_key = return_values;
					char delimiter_symbol = ','; // delimiter for the separate return values
					if(buffer_line.length() > (keyword.length()+3))
					{
						buffer_line.erase(0, keyword.length()+3);
						string word;
						for(string::size_type i=0; i<buffer_line.length(); i++)
						{
							if((buffer_line[i] == delimiter_symbol) || 
								((i == (buffer_line.length()-1)) && (word.size() > 0)))
							{
								if(i == (buffer_line.length()-1))
									if( buffer_line[i] != ',' )
										word += buffer_line[i];
								FormatString2(word); // get a single return value

								bool toAdd = false;
								if(m_fw_records.find(word) != m_fw_records.end())
									if(m_fw_records[word].Type() == T_RETURN_VALUE)
										m_fw_records[word].MakeCurrent(); // return value already in table
									else // add return value to table
										toAdd = true;
								else // add return value to table
									toAdd = true;

								if(toAdd)
								{
									FwDocParameter record(word, "ToDo", T_RETURN_VALUE);
									record.MakeCurrent();
									m_fw_records[word] = record;

									FwPrintTagParameter record1(word, TAG_RETURN_VALUE);
									m_tag_records[word] = record1;
									toAdd = false;
								}

								word.resize(0);
							}
							else
								word += buffer_line[i];
						}
					}
				}
				else
				{
					if( m_log_print ) printf("(%d)[Function block has invalid keyword inside]\nCheck function block. Warning, Parsing continuing.",(m_line_num+1));
					if( m_log_print ) printf("\n");
				}
			}
			else
			{
				FormatString1(buffer_line);

                string partial_formatted = buffer_line_unformatted;
                bool mark_pseudo_mode = pseudo_mode;
                if((buffer_line.rfind("<Pseudo>") != string::npos) && (buffer_line.rfind("</Pseudo>") != string::npos))
                {
                    if(buffer_line.rfind("<Pseudo>") > buffer_line.rfind("</Pseudo>"))
                        mark_pseudo_mode = true;
                    else
                        mark_pseudo_mode = false;

                    partial_formatted = FormatString4(buffer_line_unformatted, "<Pseudo>", "</Pseudo>");
                }
                else
                {
                    if(buffer_line.rfind("<Pseudo>") != string::npos)
                    {
                        partial_formatted = FormatString4(buffer_line_unformatted, "<Pseudo>", "</Pseudo>");
                        mark_pseudo_mode = true;
                    }
                    else if(buffer_line.rfind("</Pseudo>") != string::npos)
                    {
                        partial_formatted = FormatString4(buffer_line_unformatted, "<Pseudo>", "</Pseudo>");
                        mark_pseudo_mode = false;
                    }
                    else
                    {
                        if(pseudo_mode)
                            partial_formatted = buffer_line_unformatted;
                        else
                            partial_formatted = buffer_line;
                    }
                }

                if(current_key == tech_list)
                {
                    char delimiter_symbol = ','; // delimiter for the separate return values
					string word;
					for(string::size_type i=0; i<buffer_line.length(); i++)
					{
						if((buffer_line[i] == delimiter_symbol) || 
							((i == (buffer_line.length()-1)) && (word.size() > 0)))
						{
							if(i == (buffer_line.length()-1))
								if( buffer_line[i] != ',' )
									word += buffer_line[i];
							FormatString2(word); // get a single tech feature

                            if(word == string("REF")) word;
                            if(word == string("MT")) m_function_block.fnFeatures.threaded = 1;
                            if(word == string("SSE2")) m_function_block.fnFeatures.cp_sse2 = 1;
                            if(word == string("SSE3")) m_function_block.fnFeatures.cp_sse3 = 1;
                            if(word == string("F10H")) m_function_block.fnFeatures.cp_gh = 1;

							word.resize(0);
						}
						else
							word += buffer_line[i];
					}
                }
				else if(current_key == short_des)
					m_function_block.ShortDescriptionAppend(buffer_line);
				else if(current_key == long_des)
				{
                    if(pseudo_mode)
                    {
					    for(unsigned int i=0; i<num_line_breaks_long_desc; i++)
						    m_function_block.LongDescriptionAppend("");
                    }

					num_line_breaks_long_desc = 0;
					m_function_block.LongDescriptionAppend(partial_formatted);
                    pseudo_mode = mark_pseudo_mode;
				}
				else if(current_key == return_values)
				{
					char delimiter_symbol = ','; // delimiter for the separate return values
					string word;
					for(string::size_type i=0; i<buffer_line.length(); i++)
					{
						if((buffer_line[i] == delimiter_symbol) || 
							((i == (buffer_line.length()-1)) && (word.size() > 0)))
						{
							if(i == (buffer_line.length()-1))
								if( buffer_line[i] != ',' )
									word += buffer_line[i];
							FormatString2(word); // get a single return value

							bool toAdd = false;
							if(m_fw_records.find(word) != m_fw_records.end())
								if(m_fw_records[word].Type() == T_RETURN_VALUE)
									m_fw_records[word].MakeCurrent(); // return value already in table
								else // add return value to table
									toAdd = true;
							else // add return value to table
								toAdd = true;

							if(toAdd)
							{
								FwDocParameter record(word, "ToDo", T_RETURN_VALUE);
								record.MakeCurrent();
								m_fw_records[word] = record;

								FwPrintTagParameter record1(word, TAG_RETURN_VALUE);
								m_tag_records[word] = record1;
								toAdd = false;
							}

							word.resize(0);
						}
						else
							word += buffer_line[i];
					}
				}
				else
				{
					//if( m_log_print ) printf("(%d)[Function block has no information inside]\nCheck function block. Warning, Parsing continuing.",(m_line_num+1));
					//if( m_log_print ) printf("\n");
				}
			}

			if(exit_t)
				break;
		}

		m_buffer.resize(0);
	}

	// document data block
	void HeaderParse::DocDataBlock()
	{
		char new_line = '\n';
		string buffer_line;
		string current_parameter;
		bool exit_t = false;
		FwDocType block_type = T_DEFAULT;
		FwDocTagType tag_type = TAG_DEFAULT;
        bool always_true = true;
		while(always_true == true)
		{
			if(m_buffer.length() == 0)
				break;

			// handle one line at a time
			string::size_type new_line_pos = m_buffer.find_first_of(new_line);
			if(new_line_pos == string::npos)
			{
				buffer_line = m_buffer;
				exit_t = true;
			}
			else
			{
				buffer_line = m_buffer.substr(0, new_line_pos);
				m_buffer.erase(0, new_line_pos+1);
			}

			string delimiters = " \n\r\t";
			string::size_type lead_non_white = buffer_line.find_first_not_of(delimiters);
			if(lead_non_white == string::npos)
			{
				if(exit_t)
					break;
				else
					continue;
			}
			else
				buffer_line.erase(0, lead_non_white);

			//check if first character is #
			char keyword_start = '#'; // token to mark start of keyword
			if(buffer_line[0] == keyword_start)
			{
				FormatString1(buffer_line);
				string keyword = buffer_line;
				FormatString2(keyword);
				keyword.erase(0,1); // erase the pound sign

				string parameters = "Parameters";
				string return_values = "ReturnValues";
				string data_types = "DataTypes";
				string data_structures = "DataStructures";
				string enumerators = "Enumerators";
				if(keyword == parameters)
				{
					block_type = T_PARAMETER;
					tag_type = TAG_PARAMETER;
				}
				else if(keyword == return_values)
				{
					block_type = T_RETURN_VALUE;
					tag_type = TAG_RETURN_VALUE;
				}
				else if(keyword == data_types)
				{
					block_type = T_DATA_TYPE;
					tag_type = TAG_DATA_TYPE;
				}
				else if(keyword == data_structures)
				{
					block_type = T_DATA_STRUCTURE;
					tag_type = TAG_DATA_STRUCTURE;
				}
				else if(keyword == enumerators)
				{
					block_type = T_ENUMERATOR;
					tag_type = TAG_ENUMERATOR;
				}
				else
				{
					block_type = T_DEFAULT;
					tag_type = TAG_DEFAULT;
					if( m_log_print ) printf("(%d)[Data block has invalid keyword inside]\nCheck data block. Warning, Parsing continuing.",(m_line_num+1));
					if( m_log_print ) printf("\n");
				}
			}
			else
			{
				// check if start of definition
				char delimiter_symbol = '-'; // delimiter symbol
				FormatString1(buffer_line);
				string::size_type first_space = buffer_line.find_first_of(g_desired_space);
				if(first_space != string::npos)
					if(buffer_line.length() > (first_space+1))
						if(buffer_line[first_space+1] == delimiter_symbol)
						{
							current_parameter = buffer_line.substr(0, first_space);
							string description = "ToDo";
							if(buffer_line.length() > (first_space+3))
								description = buffer_line.substr(first_space+3);

							FwDocParameter record(current_parameter, description, block_type);
							m_fw_records[current_parameter] = record;

							FwPrintTagParameter record1(current_parameter, tag_type);
							m_tag_records[current_parameter] = record1;
						}
				else
				{
					(m_fw_records[current_parameter]).DescriptionAppend(buffer_line);
					m_buffer.resize(0);
				}
			}

			if(exit_t)
				break;
		}

		m_buffer.resize(0);
	}

	void HeaderParse::DocDocumentationBlock()
	{
        // if documentation block has <ChapterHeading> or <SectionHeading> then tag it
        for(unsigned int i=0; i<2; i++)
        {
            char *start_tag;
            char *end_tag;
            char *start_name_tag;
            char *end_name_tag;
            char *start_link_tag;
            char *end_link_tag;

            if(i == 0)
            {
                start_tag = (char*)"<SectionHeading>";
                end_tag = (char*)"</SectionHeading>";
                start_name_tag = (char*)"<SectionName>";
                end_name_tag = (char*)"</SectionName>";
                start_link_tag = (char*)"<SectionLink>";
                end_link_tag = (char*)"</SectionLink>";
            }
            else
            {
                start_tag = (char*)"<ChapterHeading>";
                end_tag = (char*)"</ChapterHeading>";
                start_name_tag = (char*)"<ChapterName>";
                end_name_tag = (char*)"</ChapterName>";
                start_link_tag = (char*)"<ChapterLink>";
                end_link_tag = (char*)"</ChapterLink>";
            }

            string m_buffer_tmp;
            while(m_buffer.find(start_tag) != string::npos)
            {
                m_buffer_tmp += m_buffer.substr(0, m_buffer.find(start_tag));
                m_buffer.erase(0, m_buffer.find(start_tag) + 16);
                string et_name = m_buffer.substr(0, m_buffer.find(end_tag));
                m_buffer.erase(0, m_buffer.find(end_tag) + 17);
			    FormatString1(et_name);

                if(i == 0)
                {
                    m_section_name = et_name;
                    m_section_tag = GenerateTag(AHL_SECTION);
                }
                else
                {
                    m_current_chapter_name = et_name;
                    m_current_chapter_tag = GenerateTag(AHL_CHAPTER);
                }

                m_buffer_tmp += string(start_tag) + "\n";
                m_buffer_tmp += string(start_name_tag); m_buffer_tmp += et_name; m_buffer_tmp += string(end_name_tag);
                m_buffer_tmp += "\n";
                    m_buffer_tmp += string(start_link_tag); 
                        if(i == 0)  m_buffer_tmp += m_section_tag;
                        else        m_buffer_tmp += m_current_chapter_tag;
                    m_buffer_tmp += string(end_link_tag);
                m_buffer_tmp += "\n";
                m_buffer_tmp += string(end_tag) + "\n";
            }
            m_buffer_tmp += m_buffer;
            m_buffer = m_buffer_tmp;
        }

		// if documentation block has </Chapter> then print function block
		if(m_buffer.find("</Chapter>") != string::npos)
		{
			string tmp = m_buffer;
			PrintAll();
			m_buffer = tmp;
		}

		while(m_buffer.length() > 0)
		{
			string::size_type pound_loc = m_buffer.find('#');

			if(pound_loc != string::npos)
			{
				m_xml_file << m_buffer.substr(0, pound_loc);
				m_buffer.erase(0, pound_loc+1);

				if(0 == m_buffer.find('@'))
				{
					string::size_type end_markup = m_buffer.find("@#");
					if(end_markup == string::npos)
					{
						printf("(%d)[Documentation block has non matching #@...@# delimiters]\nCheck documentation block. Parsing stopped.",(m_line_num+1));
						printf("\n");
						exit(1);
					}
					string word = m_buffer.substr(1, end_markup-1);
					FormatString1(word);

					map<string, FwPrintTagParameter>::const_iterator m_tr_p;
					m_tr_p = m_tag_records.find(word);
					if(m_tr_p != m_tag_records.end())
					{
						m_xml_file << m_prefix_tags[(m_tag_records[(m_tr_p->second).Name()]).Type()];
						m_xml_file << FormatString3(word);
						m_xml_file << m_suffix_tags[(m_tag_records[(m_tr_p->second).Name()]).Type()];
					}
					else
					{
						printf("(%d)[Documentation block has #@term@# not in database]\nCheck documentation block. Parsing stopped.",(m_line_num+1));
						printf("\n");
						exit(1);
					}
					m_buffer.erase(0, end_markup+2);
				}
				else if(0 == m_buffer.find("PrintAllDataTypes"))
				{
					PrintDataTypes(false);
					m_buffer.erase(0, 17); // 17 is length of PrintAllDataTypes
				}
				else if(0 == m_buffer.find("PrintAllParameters"))
				{
					PrintParameters(false);
					m_buffer.erase(0, 18); // 18 is length of PrintAllParameters
				}
				else if(0 == m_buffer.find("PrintAllReturnValues"))
				{
					PrintReturnValues(false);
					m_buffer.erase(0, 20); // 20 is length of PrintAllReturnValues
				}
				else if(0 == m_buffer.find("PrintAllDataStructures"))
				{
					PrintDataStructures(false);
					m_buffer.erase(0, 22); // 22 is length of PrintAllDataStructures
				}
				else if(0 == m_buffer.find("PrintAllEnumerators"))
				{
					PrintEnumerators(false);
					m_buffer.erase(0, 19); // 19 is length of PrintAllEnumerators
				}
				else
				{
					m_xml_file << '#';
					//m_buffer.erase(0,1);
				}
			}
			else
			{
				m_xml_file << m_buffer;
				m_buffer.resize(0);
			}
		}
	}

	// documents fw blocks
	void HeaderParse::FwKeywordBlock()
	{
		if(!m_do_doc)
		{
			printf("(%d)[Fw description block is not inside extern \"C\" block]\nCheck fw description block. Parsing stopped.",(m_line_num+1));
			printf("\n");
			exit(1);
		}

		m_names.resize(0);
		m_buffer.resize(0);
		do
		{
			m_c = (char)m_in_file.get();
			if(m_c == '\n')
				m_line_num++;

			m_buffer += m_c;
		}while(m_c != '\n'); // checking for end of line

		string out_buffer = m_buffer;

		string buffer_line = m_buffer;
		FormatString1(buffer_line);
		string keyword = m_buffer;
		FormatString2(keyword);

		string hint_block = "HintBlock";
		string include_block = "IncludeBlock";
		string function_block = "FunctionBlock";
		string data_block = "DataBlock";
		string documentation_block = "Documentation";

		m_buffer.resize(0);
		char tmp; 
		m_c = 'a'; // some initial value
		do
		{
			tmp = m_c;
			m_c = (char)m_in_file.get();
			if(m_c == '\n')
				m_line_num++;

			m_buffer += m_c;
		}while(!(tmp== '*' && m_c == '/'));

		out_buffer += m_buffer;

		m_buffer.erase(m_buffer.length()-2); // strip the ending */ character

		string m_buffer_copy = m_buffer;

		if(keyword == function_block)
		{
			if(m_fw_functions.size() > 0)
				PrintAll(); // when a function block is met print everything

			if(buffer_line.length() < (keyword.length()+3))
			{
				printf("(%d)[Function block is not followed by a name]\nCheck function block. Parsing stopped.",(m_line_num+1));
				printf("\n");
				exit(1);
			}

			buffer_line.erase(0, keyword.length()+3);
			FormatString2(buffer_line);
			m_function_block.Name(buffer_line); // set the name of current function block

			OutFileComment(buffer_line);

			FwPrintTagParameter record(buffer_line, TAG_FUNCTION);
			m_tag_records[buffer_line] = record;

			m_buffer = m_buffer_copy;
			DocFunctionBlock();
		}
		else if(keyword == data_block)
			DocDataBlock();
		else if(keyword == documentation_block)
			DocDocumentationBlock();
		else if(keyword == hint_block)
		{
			if(m_buffer.find("AutoOff") != string::npos)
				m_auto_function_block = false;
			if(m_buffer.find("AutoOn") != string::npos)
				m_auto_function_block = true;

			m_buffer.erase();
		}
		else if(keyword == include_block)
		{
			string include_tmp_buf = m_buffer;
			while(include_tmp_buf.find("#include") != string::npos)
			{
				include_tmp_buf.erase(0, include_tmp_buf.find("#include"));

				bool global_include = false;
				if(include_tmp_buf.find("#include") == include_tmp_buf.find("#includeGlobal"))
					global_include = true;

				string include_file_name;
				bool invalid_include = false;
				if(include_tmp_buf.find('"') != string::npos)
				{
					include_file_name = include_tmp_buf;
					include_file_name.erase(0, include_file_name.find('"')+1);
				}
				else
					invalid_include = true;
				if(include_file_name.find('"') != string::npos)
					include_file_name.erase(include_file_name.find('"'));
				else
					invalid_include = true;

				if(invalid_include)
				{
					printf("(%d)[Invalid include file path]\nCheck include block. Parsing stopped.", (m_line_num+1));
					exit(1);
				}

				string in_path = m_in_file_name.substr(0, m_in_file_name.find_last_of("/\\")+1);
				include_file_name = in_path + include_file_name;

				ifstream include_file(include_file_name.c_str());
				CheckFile(include_file.is_open(), include_file_name.c_str());
				string include_buffer;
				while(!include_file.eof())
					include_buffer += (char)include_file.get();

				while(include_buffer.find("/*#") != string::npos)
				{
					include_buffer.erase(0, include_buffer.find("/*#")+3);
					string which_block = include_buffer.substr(0, include_buffer.find('\n'));
					include_buffer.erase(0, include_buffer.find('\n')+1);
					m_buffer = include_buffer.substr(0, include_buffer.find("*/"));
					include_buffer.erase(0, include_buffer.find("*/")+2);

					if(which_block.find("DataBlock") != string::npos)
						DocDataBlock();
					else if(which_block.find("Documentation") != string::npos)
					{
						if(!global_include)
							DocDocumentationBlock();
					}
					else
					{
						printf("(%d)[Include file has invalid keyword block]\nCheck include file. Parsing stopped.", (m_line_num+1));
						exit(1);
					}
				}

				include_tmp_buf.erase(0, include_tmp_buf.find("#include")+8);
			}

			m_buffer.erase();
		}
		else
		{
			if( m_log_print ) printf("(%d)[Fw description block has invalid starting keyword]\nCheck description block. Warning, Parsing continuing.",(m_line_num+1));
			if( m_log_print ) printf("\n");
			m_buffer.resize(0);

			m_out_buffer += "/*#";
			m_out_buffer += out_buffer;
		}
		
		m_state = S_BASE;
	}

	// handle function declarations
	void HeaderParse::DocFunctions()
	{
		string out_buffer = m_buffer;

		if(!m_do_doc)
		{

            //// HACKKKKKKKKKKKKKKKKKKKKKKKKKKK

            m_out_buffer += out_buffer;
            m_buffer.resize(0);
            bool always_true = true;
            while(always_true == true)
		    {
			    m_c = (char)m_in_file.get();
			    m_out_buffer += m_c;
			    if(m_c == '\n')
				    m_line_num++;

			    if(m_c == ';')
				    break;
		    }
            m_c = 0;

		    m_state = S_BASE;
            return;
            ////// HACKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK


			//printf("(%d)[Function declaration is not inside extern \"C\" block]\nCheck function declaration. Parsing stopped.",(m_line_num+1));
			//printf("\n");
			//exit(1);
		}

		// get return type and function name
		m_names.resize(0);
		m_delimiters = " \r\t\n(";
		GetTwoStringsType2();

		// function name
		FwDocFunction function;
		function.Name(m_names[1]);

		// return type
		string::size_type loc;
		if((loc = m_names[0].find("STDCALL ")) != string::npos)
			m_names[0].erase(loc, loc+8); // 8 - length of STDCALL
		function.ReturnType(m_names[0]);

		FwPrintTagParameter record1(m_names[1], TAG_FUNCTION);
		m_tag_records[m_names[1]] = record1;
		FwPrintTagParameter record2(m_names[0], TAG_DATA_TYPE);
		m_tag_records[m_names[0]] = record2;

		if((m_auto_function_block) && (!IsFunctionSimilar(m_names[1], m_function_block.Name())))
		{
			PrintAll();
			AutoFunctionBlock(m_names[1]);

			// OutFileComment(m_function_block.Name());
		}

		m_out_buffer += out_buffer;

		m_names.resize(0);
		m_buffer.resize(0);
					
		// get function parameters and their types
		m_delimiters = " \r\t\n,*)";
        bool always_true = true;
        while(always_true == true)
		{
			m_c = (char)m_in_file.get();
			m_out_buffer += m_c;
			if(m_c == '\n')
				m_line_num++;

			m_buffer += m_c;
			if(m_c == '(')
			{
				printf("(%d)[Cannot have more than one open braces '(' in function declaration]\nCheck function declaration. Parsing stopped.",(m_line_num+1));
				printf("\n");
				exit(1);
			}
			if(m_c == ',' || m_c == ')')
			{
				GetTwoStringsType2();

				bool toAdd = false;
				if(m_fw_records.find(m_names[1]) != m_fw_records.end())
					if(m_fw_records[m_names[1]].Type() == T_PARAMETER)
						m_fw_records[m_names[1]].MakeCurrent(); // parameter already in table
					else // add parameter to table
						toAdd = true;
				else // add parameter to table
					if(m_names[1] != string(""))
						toAdd = true;

				if(toAdd)
				{
					FwDocParameter record(m_names[1], "ToDo", T_PARAMETER);
					record.MakeCurrent();
					m_fw_records[m_names[1]] = record;
					toAdd = false;
				}

				string type = m_names[0];
				FormatString1(type);
				string name = m_names[1];
				FormatString1(name);

				FwPrintTagParameter record1(name, TAG_PARAMETER);
				m_tag_records[name] = record1;
				FwPrintTagParameter record2(type, TAG_DATA_TYPE);
				m_tag_records[type] = record2;

				StringParameter parameter;
				parameter.m_name = name;
				parameter.m_type = type;
				function.AddParameter(parameter);

				m_names.resize(0);

				if(m_c == ')')
					break;
			}
		}

        while(always_true == true)
		{
			m_c = (char)m_in_file.get();
			m_out_buffer += m_c;
			if(m_c == '\n')
				m_line_num++;

			if(m_c == ';')
				break;
		}

		m_fw_functions.push_back(function);
		m_state = S_BASE;
	}

	void HeaderParse::OutFileComment(const string& buffer)
	{
		m_out_buffer += "\n\n";
		for(int i=0; i<64; i++) m_out_buffer += '/';
		m_out_buffer += '\n'; m_out_buffer += "//";

		for(int i=0; i<24; i++) m_out_buffer += ' ';
		m_out_buffer += buffer;
		
		m_out_buffer += '\n';
		for(int i=0; i<64; i++) m_out_buffer += '/';
		m_out_buffer += '\n';
	}

	void HeaderParse::Text()
	{
		string out_buffer = m_buffer;

		if(m_c == '\n')
			m_line_num++;

		switch(m_state)
		{
			case S_LINE_COMMENT:  break; // do nothing.
			case S_BLOCK_COMMENT: break; // do nothing.
			case S_FW_BLOCK:	  
				//TODO: do something here.
				break;

			case S_BASE:
				if(m_c == '{')
				{
					if(m_do_doc) // do not allow { in S_BASE state
					{
						printf("(%d)[Cannot have more than one open braces inside extern \"C\" block]\nCheck the input header file. Parsing stopped.",(m_line_num+1));
						printf("\n");
						exit(1);
					}

					const char* delimiters = " \n\r\t{";

					string::size_type  lead_non_white = m_buffer.find_first_not_of(delimiters);
					m_buffer.erase(0, lead_non_white);

					string::size_type trail_non_white = m_buffer.find_last_not_of(delimiters);
					m_buffer.erase(trail_non_white+1);

					string extern_c = "extern \"C\"";
					if(m_buffer == extern_c)
						m_do_doc = true;	// activate documentation

					m_out_buffer += out_buffer;
					m_buffer.resize(0);
					if(!m_do_doc)
					{
						m_buffer.resize(0);
						m_braces_count = 1;
						Braces();
						m_c = '{';
					}
				}
				
				if(m_c == '(')
				{
					m_state = S_FUNCTION;
					DocFunctions();
				}
				if(m_c == ';')
				{
					if(!m_do_doc) m_out_buffer += out_buffer;
					m_buffer.resize(0);
				}

				if(m_c == '}')
					m_do_doc = false; // deactivate documentation

				break;

			//default: 
				//cout << char(10); //m_c; 
		}
	}

	// change state accordingly when slash sign is met
	void HeaderParse::Slash()
	{
		switch(m_state)
		{
			case S_BASE:
				m_state = S_SLASH;
				break;
			case S_SLASH:
				m_state = S_LINE_COMMENT;
				break;
			case S_BLOCK_COMMENT_STAR:
				m_state = S_BASE;
				break;	
			default:
				Text();
		}
	}

	// change state accordingly when star sign is met
	void HeaderParse::Star()
	{
		switch(m_state)
		{
			case S_SLASH:
				m_state = S_BLOCK_COMMENT;
				break;

			case S_BLOCK_COMMENT:
				m_state = S_BLOCK_COMMENT_STAR;
				break;

			default:
				Text();
		}
	}

	// change state accordingly when pound sign is met
	void HeaderParse::Pound()
	{
		switch(m_state)
		{
			case S_BLOCK_COMMENT:
				m_state = S_FW_BLOCK;
				FwKeywordBlock(); // call function to start printing
				break;
			case S_BASE:
				m_state = S_POUND_DIRECTIVE;
				break;

			default:
				Text();
		}
	}

	// change state accordingly when end of line is met
	void HeaderParse::EndOfLine()
	{
		if(m_state != S_BASE && m_state != S_POUND_DIRECTIVE)
		{
			m_out_buffer += m_buffer;
			m_buffer.resize(0);
		}

		switch(m_state)
		{
			case S_LINE_COMMENT: 
				m_state = S_BASE;
				m_line_num++;
				break;

			case S_POUND_DIRECTIVE:
				m_state = S_BASE;
				m_line_num++;
				PoundInactive();
				m_buffer.resize(0);
				break;

			default:
				Text();
		}
	}

	void HeaderParse::PoundInactive()
	{
		string out_buffer = m_buffer;

		FormatString1(m_buffer);
		string::size_type loc = m_buffer.find("#if BUILD_NUM_AT_LEAST");
		if(loc != string::npos)
		{
			m_buffer.erase(0, loc + sizeof("#if BUILD_NUM_AT_LEAST") - 1);
			loc = m_buffer.find_first_of("0123456789");
			if(loc != string::npos)
				m_buffer.erase(0, loc);
			else
			{
				printf("(%d)[No version number following BUILD_NUM_AT_LEAST]\nCheck the input header file. Parsing stopped.",(m_line_num+1));
				printf("\n");
				exit(1);
			}
			loc = m_buffer.find_first_not_of("0123456789");
			if(loc != string::npos) m_buffer = m_buffer.substr(0, loc);
			unsigned int version = atoi(m_buffer.c_str());
			if(version > m_version_num)					// do nothing until you hit matching #endif
			{
				string buffer;
				char tmp; 
				char c = 'a'; // some initial value
				do
				{
					bool process = false;
					tmp = c;
					c = (char)m_in_file.get();
					buffer += c;
					if(c == '\n')
						m_line_num++;

					if( c=='/' && tmp=='/' ) // to get out of line comment
					{
						c = 'a';
						buffer.resize(buffer.length()-2);
						process = true;
						char d = 'a'; // some initial value
						while( d != '\n' )
							d = (char)m_in_file.get();

						m_line_num++;
					}

					if( tmp=='/' && c=='*' )
					{
						buffer.resize(buffer.length()-2);
						process = true;
						char tmp1;
						char d = 'a'; // some initial value
						do
						{
							tmp1 = d;
							d = (char)m_in_file.get();
							if(d == '\n')
								m_line_num++;
						}while(!(tmp1=='*' && d=='/')); // to get out of block comment
					}

					if( c == '\n' )
						process = true;

					if(process)
					{
						FormatString1(buffer);
						string::size_type loc = buffer.find("#endif");
						if(loc != string::npos) break;
						buffer.resize(0);
					}
					process = false;
				}while(!m_in_file.eof());
			}
			else
				m_pound_inactive = true;
		}
		else
		{
			if(m_pound_inactive)
			{
				if(m_buffer.find("#endif") != string::npos)
					m_pound_inactive = false;
				else
					m_out_buffer += out_buffer;
			}
			else
			{
				string::size_type loc = out_buffer.find("../fwBase/fwBase.h");
				if(loc != string::npos) out_buffer.replace(loc, 20, "fwBase.h"); // 20 - length of ../fwBase/fwBase.h
				m_out_buffer += out_buffer;
			}
		}
	}

	void HeaderParse::ParsePch()
	{
		string buffer;
		char tmp; 
		char c = 'a'; // some initial value
		do
		{
			bool process = false;
			tmp = c;
			c = (char)m_pch_file.get();
			buffer += c;

			if( c=='/' && tmp=='/' ) // to get out of line comment
			{
				c = 'a';
				buffer.resize(buffer.length()-2);
				process = true;
				char d = 'a'; // some initial value
				while( d != '\n' )
					d = (char)m_pch_file.get();
			}

			if( c=='*' && tmp=='/' )
			{
				buffer.resize(buffer.length()-2);
				process = true;
				char tmp1;
				char d = 'a'; // some initial value
				do
				{
					tmp1 = d;
					d = (char)m_pch_file.get();
				}while(!(tmp1=='*' && d=='/')); // to get out of block comment
			}

			if( c == '\n' )
				process = true;

			if(process)
			{
				FormatString1(buffer);
				string::size_type loc;
				if((loc = buffer.find("#define FW_BUILD_NUM")) != string::npos)
				{
					buffer.erase(0, loc + sizeof("#define FW_BUILD_NUM") - 1);
					loc = buffer.find_first_of("0123456789");
					if(loc != string::npos)
						buffer.erase(0, loc);
					else
					{
						printf("[No version number following FW_BUILD_NUM]\nCheck the input buildnum file. Parsing stopped.");
						printf("\n");
						exit(1);
					}
					loc = buffer.find_first_not_of("0123456789");
					if(loc != string::npos) buffer = buffer.substr(0, loc);
					m_version_num = atoi(buffer.c_str());
				}
				else if((loc = buffer.find("#define IPP_NAMESPACE_EXISTING")) != string::npos)
					m_ipp = true;
				else if((loc = buffer.find("#define IPP_NAMESPACE_ALL")) != string::npos)
				{
					buffer.erase(0, loc + 25); // 25 - length of #define IPP_NAMESPACE_ALL
					if(buffer.find('"') != string::npos) buffer.erase(0, buffer.find('"') + 1);
					if(buffer.find('"') != string::npos) buffer.erase(buffer.find('"'));
					string ipp_file_name = m_def_file_name;
					if(ipp_file_name.find('/') != string::npos) ipp_file_name.erase(0, (ipp_file_name.find_last_of("/")) + 1);
					if(ipp_file_name.find('\\') != string::npos) ipp_file_name.erase(0, (ipp_file_name.find_last_of("\\")) + 1);
					ipp_file_name = buffer + '/' + ipp_file_name + ".ipp";

					ifstream ipp_file(ipp_file_name.c_str());
					if(ipp_file.is_open()) 
					{
						m_ipp = true;
						m_ipp_symbol_file_name = ipp_file_name + ".def";
					}
				}
				buffer.resize(0);
			}

			process = false;

		}while(!m_pch_file.eof());
	}

	void HeaderParse::Parse()
	{
		ParsePch();

		char old_m_c=0;
		while(!m_in_file.eof())
		{
			m_c = (char)m_in_file.get();
			m_buffer += m_c;

			switch(m_c)
			{
				case '/':	Slash();							break;
				case '*':	Star();								break;
				case '#':	Pound();							break;
				case 10:	if(old_m_c != '\\') EndOfLine();	break;
				
				default:	Text();
			}
			old_m_c = m_c;
		}
		if(m_buffer.length() > 0) m_buffer.resize(m_buffer.length() - 1);
		m_out_buffer += m_buffer;
	}

	void HeaderParse::IppFwDef()
	{
		if(m_ipp)
		{
			ifstream fw_file(m_def_file_name.c_str()); CheckFile(fw_file.is_open(), m_def_file_name.c_str());
			
			string sqbdef_name = m_def_file_name + ".sqb.ipp";
			ofstream sqbdef(sqbdef_name.c_str()); CheckFile(sqbdef.is_open(), sqbdef_name.c_str());

			string out;
			set<string> ipp_exist;
			string line;
			while(getline(fw_file, line))
				if(line.find("fw") == 0)
				{
					string ipp_fn = "ipp"; ipp_fn += line.substr(2); // 2 - length of "fw"
					FormatString1(ipp_fn);
					sqbdef << "[" << ipp_fn << "]\n";
					ipp_exist.insert(ipp_fn);
					out += line + "\n";
					out += ipp_fn + "\t\t\t\t\t\t=" + line + "\n";
				}
				else
					out += line + "\n";

			sqbdef.close();

			out += "\n\n";

			string dummy_func;
			if(ipp_exist.size() > 0)
			{
				dummy_func = *(ipp_exist.begin());
				dummy_func = "fw" + dummy_func.substr(3); // 3 -length of "ipp"
			}

			if(m_ipp_symbol_file_name.length() > 0)
			{
				ifstream ipp_file(m_ipp_symbol_file_name.c_str()); CheckFile(ipp_file.is_open(), m_ipp_symbol_file_name.c_str());
				while(getline(ipp_file, line))
					if(line.find("ipp") == 0)
					{
						FormatString1(line);
						if(ipp_exist.find(line) == ipp_exist.end())
							out += line + "\t\t\t\t\t\t=" + dummy_func + "\n";
					}
				ipp_file.close();
			}

			fw_file.close();

			ofstream out_file(m_def_file_name.c_str()); CheckFile(out_file.is_open(), m_def_file_name.c_str());
			out_file << out;
			out_file.close();
		}
	}

	void HeaderParse::SplitXmlFile()
	{
		ifstream xml_file(m_xml_file_name.c_str()); CheckFile(xml_file.is_open(), m_xml_file_name.c_str());
		string xml_file_buffer, line;
		while(getline(xml_file, line))
		{
			xml_file_buffer += line;
			xml_file_buffer += "\n";
		}

		string header;
		//header = xml_file_buffer.substr(0, xml_file_buffer.find("<Chapter>"));
		header = xml_file_buffer.substr(0, xml_file_buffer.find("<Section>")+8); header = header + string("\n\n");
		xml_file_buffer.erase(0, xml_file_buffer.find("<Chapter>")); // 9 - length of <Chapter>

		std::vector<string> split_xml_file_names;
		while(xml_file_buffer.find("</Chapter>") != string::npos)
		{
			string chapter_buffer = xml_file_buffer.substr(0, xml_file_buffer.find("</Chapter>") + 10); // 10 - length of </Chapter>
			xml_file_buffer.erase(0, xml_file_buffer.find("</Chapter>") + 10); 

			string chapter_name = chapter_buffer.substr(0, chapter_buffer.find("</ChapterName>"));
			chapter_name.erase(0, chapter_name.find("<ChapterName>") + 14); // 13 - length of <ChapterName>
			FormatString1(chapter_name);
			for(string::size_type i=0; i<chapter_name.length(); i++)
				if(chapter_name[i] == ' ')
					chapter_name[i] = '_';

			string chapter_file_name = m_xml_file_name + "." + chapter_name + ".xml";
			split_xml_file_names.push_back(chapter_file_name);
			ofstream chapter_file(chapter_file_name.c_str()); CheckFile(chapter_file.is_open(), chapter_file_name.c_str());
			chapter_file << header << chapter_buffer << "\n";
			chapter_file.close();
		}

		//string footer = xml_file_buffer;
		string footer = xml_file_buffer.erase(0, xml_file_buffer.find("</Section>")); footer = string("\n\n") + footer;
		for(unsigned int i=0; i<split_xml_file_names.size(); i++)
		{
			ofstream chapter_file(split_xml_file_names[i].c_str(), ios::app); CheckFile(chapter_file.is_open(), split_xml_file_names[i].c_str());
			chapter_file << footer;
			chapter_file.close();
		}
	}

    void CreateGlobalFnIndex(const char *file_name)
    {
        ofstream index_file(file_name);
        CheckFile(index_file.is_open(), file_name);

        index_file << "<Section>\n\n";
        index_file << "<SectionHeading>\n";
        index_file << "<SectionName>Function Quick Reference</SectionName>\n";
        index_file << "<SectionLink>"; index_file << GenerateTag(AHL_SECTION);index_file << "</SectionLink>\n";
        index_file << "</SectionHeading>\n\n";


        index_file << "<FunctionQuickReference>\n\n";

        index_file << "<QRColHead>Function</QRColHead>\n";
        index_file << "<QRColHead>Chapter</QRColHead>\n";
        index_file << "<QRColHead>Section</QRColHead>\n";
        index_file << "<QRColHead>MT</QRColHead>\n";
        index_file << "<QRColHead>SSE2</QRColHead>\n";
        index_file << "<QRColHead>SSE3</QRColHead>\n";
        index_file << "<QRColHead>Family10h</QRColHead>\n\n\n";

        std::sort(g_fn_index.begin(), g_fn_index.end());

        for(vector<FunctionIndexEntry>::const_iterator i=g_fn_index.begin(); i != g_fn_index.end(); i++)
        {
            index_file << "<QRRow>\n";

            index_file << "\t<QRFunction>\n";
            index_file << "\t\t<QRFunctionName>" << (*i).name << "</QRFunctionName>\n";
            index_file << "\t\t<QRFunctionLink>" << (*i).tag << "</QRFunctionLink>\n";
            index_file << "\t</QRFunction>\n";

            index_file << "\t<QRChapter>\n";
            index_file << "\t\t<QRChapterName>" << (*i).chapter_name << "</QRChapterName>\n";
            index_file << "\t\t<QRChapterLink>" << (*i).chapter_tag << "</QRChapterLink>\n";
            index_file << "\t</QRChapter>\n";

            index_file << "\t<QRSection>\n";
            index_file << "\t\t<QRSectionName>" << (*i).section_name << "</QRSectionName>\n";
            index_file << "\t\t<QRSectionLink>" << (*i).section_tag << "</QRSectionLink>\n";
            index_file << "\t</QRSection>\n\n";

            index_file << "\t<QRCell>"; if((*i).fnFeatures.threaded) index_file << "X"; else index_file << "&#160;"; index_file << "</QRCell>\n";
            index_file << "\t<QRCell>"; if((*i).fnFeatures.cp_sse2)  index_file << "X"; else index_file << "&#160;"; index_file << "</QRCell>\n";
            index_file << "\t<QRCell>"; if((*i).fnFeatures.cp_sse3)  index_file << "X"; else index_file << "&#160;"; index_file << "</QRCell>\n";
            index_file << "\t<QRCell>"; if((*i).fnFeatures.cp_gh)    index_file << "X"; else index_file << "&#160;"; index_file << "</QRCell>\n";
		
            index_file << "</QRRow>\n\n";
        }

        index_file << "\n\n</FunctionQuickReference>\n";

        index_file << "\n\n</Section>\n\n";
        index_file.close();
    }
};

void JoinFiles(const char *out_file_name, ...)
{
    va_list vlist;
    va_start(vlist, out_file_name);

    ofstream out_file(out_file_name);
    CheckFile(out_file.is_open(), out_file_name);

    out_file << "<?xml version=\"1.0\" standalone=\"yes\"?>" << endl;
    out_file << "<?xml-stylesheet type=\"text/xsl\" href=\"fw_transform.xsl\"?>" << endl;
    
    out_file << "\n\n";
    out_file << "<Manual>\n\n\n";

    bool always_true = true;
    while(always_true == true)
    {
        const char *file_name = va_arg(vlist, const char *);
        if(file_name == NULL)
            break;

        ifstream file_tmp(file_name);
        CheckFile(file_tmp.is_open(), file_name);

        string line;
        while(getline(file_tmp, line))
            out_file << line << endl;
    }

    out_file << "\n\n\n";
    out_file << "</Manual>\n\n";

    va_end(vlist);
}

void Usage()
{
    printf("\n Usage: A. single header B. all headers\n");
    printf("\n Usage A:\n");
	printf("\n Usage:1. FwHeaderConvert -xml -def \"input header file\" \"input buildnum file\" \"output header file\" \"output xml file\" \"output def file\" \"def dll name\"");
	printf("\n Usage:2. FwHeaderConvert -xml \"input header file\" \"input buildnum file\" \"output header file\" \"output xml file\"");
	printf("\n Usage:3. FwHeaderConvert -def \"input header file\" \"input buildnum file\" \"output header file\" \"output def file\" \"def dll name\"");
    printf("\n");
	printf("\n E.g:1. FwHeaderConvert -xml -def input.h buildnum.h output.h output.xml output.def output.dll");
	printf("\n E.g:2. FwHeaderConvert -xml input.h buildnum.h output.h output.xml");
	printf("\n E.g:3. FwHeaderConvert -def input.h buildnum.h output.h output.def output.dll");
	printf("\n");
	printf("\n Usage B:\n");
    printf("\n Usage: FwHeaderConvert -all fw_doc.xml domain/common/include/buildnum.h doc/fwIntroDoc.h domain/fwBase/include/fwBase.h domain/fwFFT/include/fwFFT.h domain/fwImage/include/fwImage.h domain/fwJPEG/include/fwJPEG.h domain/fwSignal/include/fwSignal.h domain/fwVideo/include/fwVideo.h\n");
    printf("\n");

	exit(1);
}

int HC_CDECL main(int argc, char* argv[])
{
    if( (argc == 11) && (string(argv[1]) == string("-all")) )
    {
        g_all_headers_mode = true;

        string tmp_intro_h = argv[4]; tmp_intro_h += ".out.h";
        string tmp_intro_xml = argv[4]; tmp_intro_xml += ".xml";
        string tmp_base_h = argv[5]; tmp_base_h += ".out.h";
        string tmp_base_xml = argv[5]; tmp_base_xml += ".xml";
        string tmp_fft_h = argv[6]; tmp_fft_h += ".out.h";
        string tmp_fft_xml = argv[6]; tmp_fft_xml += ".xml";
        string tmp_image_h = argv[7]; tmp_image_h += ".out.h";
        string tmp_image_xml = argv[7]; tmp_image_xml += ".xml";
        string tmp_jpeg_h = argv[8]; tmp_jpeg_h += ".out.h";
        string tmp_jpeg_xml = argv[8]; tmp_jpeg_xml += ".xml";
        string tmp_signal_h = argv[9]; tmp_signal_h += ".out.h";
        string tmp_signal_xml = argv[9]; tmp_signal_xml += ".xml";
        string tmp_video_h = argv[10]; tmp_video_h += ".out.h";
        string tmp_video_xml = argv[10]; tmp_video_xml += ".xml";

        string tmp_global_index = argv[2]; tmp_global_index += ".fn_index";

        {
            HeaderParse hp_intro(argv[4], argv[3], tmp_intro_h.c_str(), tmp_intro_xml.c_str());
		    hp_intro.Parse();

            {
		    HeaderParse hp_base(argv[5], argv[3], tmp_base_h.c_str(), tmp_base_xml.c_str());
		    hp_base.Parse();
            }

            HeaderParse hp_fft(argv[6], argv[3], tmp_fft_h.c_str(), tmp_fft_xml.c_str());
		    hp_fft.Parse();

            HeaderParse hp_image(argv[7], argv[3], tmp_image_h.c_str(), tmp_image_xml.c_str());
		    hp_image.Parse();
            
            HeaderParse hp_jpeg(argv[8], argv[3], tmp_jpeg_h.c_str(), tmp_jpeg_xml.c_str());
		    hp_jpeg.Parse();

            HeaderParse hp_signal(argv[9], argv[3], tmp_signal_h.c_str(), tmp_signal_xml.c_str());
		    hp_signal.Parse();

            HeaderParse hp_video(argv[10], argv[3], tmp_video_h.c_str(), tmp_video_xml.c_str());
		    hp_video.Parse();

            CreateGlobalFnIndex(tmp_global_index.c_str());
        }

        JoinFiles(argv[2], tmp_intro_xml.c_str(), tmp_base_xml.c_str(), tmp_fft_xml.c_str(), tmp_image_xml.c_str(), tmp_jpeg_xml.c_str(), tmp_signal_xml.c_str(), tmp_video_xml.c_str(), tmp_global_index.c_str(), NULL);
    }
	else if( (argc == 9) && (string(argv[1]) == string("-xml")) && (string(argv[2]) == string("-def")) )
	{
		HeaderParse hp(argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
		hp.Parse();
	}
	else if( (argc == 6) && (string(argv[1]) == string("-xml")) )
	{
		HeaderParse hp(argv[2], argv[3], argv[4], argv[5]);
		hp.Parse();
	}
	else if( (argc == 7) && (string(argv[1]) == string("-def")) )
	{
		HeaderParse hp(argv[2], argv[3], argv[4], argv[5], argv[6]);
		hp.Parse();
	}
	else
		Usage();

	return 0;
}

