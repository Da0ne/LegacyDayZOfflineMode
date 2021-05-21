class ItemVariableFlags
{
	static const int NONE 					= 0;
	static const int FLOAT			 		= 0x0001;
	static const int STRING		 			= 0x0002;
	static const int BOOL 					= 0x0004;
};

class CashedObjectsParams
{	
	static Param1<int> 		PARAM1_INT;//CashedObjectsParams.PARAM1_INT
	static Param1<float> 	PARAM1_FLOAT;//CashedObjectsParams.PARAM1_FLOAT
	static Param1<string> 	PARAM1_STRING;//CashedObjectsParams.PARAM1_STRING
	
	static Param2<int,int> 		PARAM2_INT_INT;//CashedObjectsParams.PARAM2_INT_INT
	static Param2<int,float> 	PARAM2_INT_FLOAT;//CashedObjectsParams.PARAM2_INT_FLOAT
	static Param2<int,string> 	PARAM2_INT_STRING;//CashedObjectsParams.PARAM2_INT_STRING
	static Param2<string,float> PARAM2_STRING_FLOAT;//CashedObjectsParams.PARAM2_STRING_FLOAT
	static Param2<string,string> PARAM2_STRING_STRING;//CashedObjectsParams.PARAM2_STRING_STRING
	
	void CashedObjectsParams()
	{
		PARAM1_INT = new Param1<int>(0);
		PARAM1_FLOAT = new Param1<float>(0);
		PARAM1_STRING = new Param1<string>("");
		
		PARAM2_INT_INT = new Param2<int,int>(0,0);
		PARAM2_INT_FLOAT = new Param2<int,float>(0,0);
		PARAM2_INT_STRING = new Param2<int,string>(0,"");
		PARAM2_STRING_FLOAT = new Param2<string,float>("",0);
		PARAM2_STRING_STRING = new Param2<string,string>("","");
	
	}
	void ~CashedObjectsParams()
	{
		delete PARAM1_INT;
		delete PARAM1_FLOAT;
		delete PARAM1_STRING;
		
		delete PARAM2_INT_INT;
		delete PARAM2_INT_FLOAT;
		delete PARAM2_INT_STRING;
		delete PARAM2_STRING_FLOAT;
		delete PARAM2_STRING_STRING;
	}
};


class CashedObjectsArrays//don't forget to .Clear() your cache object before using it
{	
	void CashedObjectsArrays()
	{
		ARRAY_STRING 	= new TStringArray;
		ARRAY_FLOAT 	= new TFloatArray;
		ARRAY_INT 		= new TIntArray;
	}
	
	void ~CashedObjectsArrays()
	{
		delete ARRAY_STRING;
		delete ARRAY_FLOAT;
		delete ARRAY_INT;
	}
	
	static TStringArray ARRAY_STRING;//CashedObjectsArrays.ARRAY_STRING
	static TFloatArray ARRAY_FLOAT;//CashedObjectsArrays.ARRAY_FLOAT
	static TIntArray ARRAY_INT;//CashedObjectsArrays.ARRAY_INT
};