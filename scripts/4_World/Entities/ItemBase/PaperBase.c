typedef Param2<string, EntityAI> WritePaperParams;

class Consumable_Paper extends ItemBase
{
	// EntityAI override
	void OnRPC( int rpc_type, ParamsReadContext  ctx)
	{
		super.OnRPC(rpc_type, ctx);
		
		if (rpc_type == RPC_WRITE_NOTE)
		{
			autoptr WritePaperParams param = new WritePaperParams("", NULL); // message, pen
			
			if (ctx.Read(param))
			{
				string message;
				GetVariable("message", message);
				// remove any html tags added by user
				GetGame().HtmlToPlain(param.param1);
				param.param1.Replace("\n", "<br>");
				
				if (message != "") message += "<br>";
				message +=  "<font color=\"" + param.param2.ConfigGetString("writingColor") + "\">" + param.param1 + "</font>";
				
				if (message.Length() < 1000) 
				{
					SetVariable("message", message);
				}
			}
		}
	}
}
