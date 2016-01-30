#include "stdafx.h"
#include "CppUnitTest.h"

// Tested in another file
#include "../../../../libraries/FileIO/UserInput.h"
#include "../../../../libraries/FileIO/PrintOut.h"
#include "../../../../libraries/FileIO/FileErrors.h"
#include "../../../../libraries/FileIO/FileIn.h"	
#include "../../../../libraries/FileIO/FileOut.h"

// Tested in this file
#include "../../../../libraries/FileIO/FilePath.h"
#include "../../../../libraries/FileIO/Conversion.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FileIOTest
{
	TEST_CLASS(FileInTest){
public:
TEST_METHOD(separator_detect){
	Assert::AreEqual(std::string("\t"),FileIn::detect_separator("blah.xls"));
	Assert::AreEqual(std::string(","),FileIn::detect_separator("blah.csv"));
}
	};

	TEST_CLASS(FilePathTest){
public:
	TEST_METHOD(csv_strip_extension){
		std::string blah = "blah.csv";
		FilePath::strip_extension(blah);
		Assert::AreEqual(blah,std::string("blah"));
	}

	TEST_METHOD(csv_strip_filepath){
		std::string blah = "blah/blah/bkjsgk/blah.csv";
		FilePath::strip_file_path(blah);
		Assert::AreEqual(blah,std::string("blah.csv"));
	}

	};

	TEST_CLASS(ConversionTest)
	{
	public:
		
		TEST_METHOD(int_dbl_element_conversion)
		{
			int foo=1;
			double bar = Conversion::convert_element<double>(foo);

			Assert::AreEqual(foo,int(bar));//,L"blah",LINE_INFO());
		}
		TEST_METHOD(int_str_element_conversion)
		{
			int foo=1;
			std::string c = Conversion::convert_element<std::string>(foo);
			Assert::AreEqual(c,std::string("1"));
		}
		TEST_METHOD(str_int_element_conversion)
		{
			std::string foo="6";
			int c = Conversion::convert_element<int>(foo);
			Assert::AreEqual(c,6);
		}
		TEST_METHOD(int_dbl_vector_conversion){
			std::vector<std::vector<int> > a(10,std::vector<int>(10,60));
			std::vector<std::vector<double> > b;
			b = Conversion::convert_vector<double>(a);

			for (int i=0; i<a.size(); i++){
				for (int j=0; j<a[i].size(); j++){
					Assert::AreEqual(a[i][j],int(b[i][j]));
				}
			}
		}
		TEST_METHOD(int_str_vector_conversion){
			std::vector<std::vector<int> > a(10,std::vector<int>(10,60));
			std::vector<std::vector<std::string> > b;
			b = Conversion::convert_vector<std::string>(a);

			for (int i=0; i<a.size(); i++){
				for (int j=0; j<a[i].size(); j++){
					Assert::AreEqual(b[i][j],std::string("60"));
				}
			}
		}
		TEST_METHOD(str_int_vector_conversion){
			std::vector<std::vector<std::string> > a(10,std::vector<std::string>(10,"60"));
			std::vector<std::vector<int> > b;
			b = Conversion::convert_vector<int>(a);

			for (int i=0; i<a.size(); i++){
				for (int j=0; j<a[i].size(); j++){
					Assert::AreEqual(b[i][j],60);
				}
			}
		}
		TEST_METHOD(str_divide){
			string_matrix1d blah = Conversion::divide("cats_are_the_best","_");
			Assert::AreEqual(blah[0],std::string("cats"));
			Assert::AreEqual(blah[1],std::string("are"));
			Assert::AreEqual(blah[2],std::string("the"));
			Assert::AreEqual(blah[3],std::string("best"));
		}
	};
}