a simple online mobile game server server .

gate is a tcp connection manager
	it report tcp event to logic server
agent server deal the message on the connection .
	every connection map a session .
	a session map a agent dealing the message .
service emplementes some conncrete service.

--------------------------------------------
base support the app common tools helping 
wirting a good server .
------------------------------------------------


  1-2    2-2     3-3
client  client  client  ____
 |_______|       |          |_________GLB_______DIR_______VER______CDN
 |       |       |
_|_      |       |
 |       |       |
 1       2       3
gate    gate    gate  ------------------- open platform
                            \                     /\
 \      / \      /           Auth ________________||             

   \  /    \   /
     1       2 
   agent    agent    TASK   BATTLE  ACHIEVEMENT   
  |____|   |____|    VER    ACCOUNT ANTI CHEAT
     |      |           
      \    /      LBS    SNS     RANK     OSS
       \  /_______|______|________|________|
        -
       Redis --- Redis
        |
 level DB/mysql DB  ---------- OLAP 










	
