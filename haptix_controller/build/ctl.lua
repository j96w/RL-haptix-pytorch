abs = require'libarmctl'

abs.luaArmstart()
--abs.luaPrintstate()
--abs.luaPrintrobotinfo()
--local a ={abs.luaGetforce0(), abs.luaGetforce1(), abs.luaGetforce2(), 
--	  abs.luaGetforce3(), abs.luaGetforce4(), abs.luaGetforce5(), 
--          abs.luaGetforce6(), abs.luaGetforce7(), abs.luaGetforce8(), 
--          abs.luaGetforce9(), abs.luaGetforce10(), abs.luaGetforce11(), 
--          abs.luaGetforce12(), abs.luaGetforce13(), abs.luaGetforce14(), 
--          abs.luaGetforce15(), abs.luaGetforce16(), abs.luaGetforce17(),
--	  abs.luaGetforce18()}
--for i=1, 19, 1 do
--	print(a[i])
--end
--abs.luaArmstop()
abs.luaHanddown(0.0);
abs.luaFinger1(350.0);
abs.luaHandrotate(0.0);
abs.luaFinger2down(350.0);
abs.luaFinger3down(350.0)
abs.luaFinger4down(350.0)
abs.luaFinger5down(350.0);
