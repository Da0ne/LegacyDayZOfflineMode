static string GetDate()
{
	int year, month, day, hour, minute, second;
	
	GetYearMonthDay(year, month, day);
	GetHourMinuteSecond(hour, minute, second);
	string date = month.ToStringLen(2) + "-" + day.ToStringLen(2) + "-" + hour.ToStringLen(2) + minute.ToStringLen(2);
	
	return date;
}

static void DataBaseDelete(string in1, string in2)
{
	string player_alive, player_dead, file_name, date;
	
	MakeDirectory("$profile:db" + "\\dead\\" + in2);
	
	file_name = "$FILENAME$.sqf";
	file_name.Replace("$FILENAME$", in1);
	date = GetDate();
	player_alive = "$profile:db" + "\\alive\\" + in2 + "\\" + file_name;
	player_dead = "$profile:db" + "\\dead\\" + in2 + "\\" + file_name;
	FileHandle file = OpenFile(player_alive, FileMode.READ);
	
	if (file != 0)
	{   
		CloseFile(file);
		CopyFile(player_alive, player_dead); 	
		DeleteFile(player_alive);
	} 
	Print("DataBase: DataBaseDelete()");
}

static void DataBaseWrite(string in1, string in2, string in3)
{	
	string player_alive, file_name;

	file_name = "$FILENAME$.sqf";
	file_name.Replace("$FILENAME$", in1);
	player_alive = "$profile:db" + "\\alive\\" + in2 + "\\" + file_name;	
	FileHandle file = OpenFile(player_alive, FileMode.WRITE);
	
	if (file != 0)
	{
		in3.Replace("<null>", "\'" + "\'");
		FPrintln(file, in3);
		CloseFile(file);

	}else{
		MakeDirectory("$profile:db" + "\\alive\\" + in2 );
	}
	Print("DataBase: DataBaseWrite()");
}

static string DataBaseRead(string in1, string in2)
{
	string player_alive, file_content, file_name;
	
	file_content = "";
	file_name = "$FILENAME$.sqf";
	file_name.Replace("$FILENAME$", in1);
	player_alive = "$profile:db" + "\\alive\\" + in2 + "\\" + file_name;
		
	FileHandle file = OpenFile(player_alive, FileMode.READ);
	
	if (file != 0)
	{  
		FGets(file, file_content);
		CloseFile(file);
	}
	
	return file_content;
	Print("DataBase: DataBaseRead()");
	Print(file_content);
}