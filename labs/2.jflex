%%

%standalone

%class Lexer
%unicode

scheme = ftp|http|gopher|uk
letter = [^\n\r%/<>:.\\#]|{}
name = ({letter}+)\.({letter}+)\.({letter}+)
ipaddresscomp = [0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]
ipaddress = ({ipaddresscomp}.){3}{ipaddresscomp}
domain = {name}|{ipaddresscomp} 
port = [1-9][0-9]{0,3}
filename = ({letter}+)\.({letter}+)
anchor = [a-zA-Z0-9]+
escape = "%"[0-9A-F]{2}

%%

^({scheme}\:\/\/{domain}\/)(\:{port})?(\/{filename})(\#{anchor})

{

	System.out.println("recognize!"+yytext());
}

.* {

	System.out.println("uncorrect:"+yytext());
}

{nl}   {;}
