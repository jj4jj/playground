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


 1-2     2-2     3-3
client  client  client  ____
 |_______|       |          |_________GLB_______DIR_______VER[CDN]
 |       |       |
_|_      |       |
 |       |       |
 1       2       3
gate    gate    gate  ------------------------ open platform
                            \                           /\
 \      / \      /           Auth _______Account________||             

   \  /    \   /
     1       2 
   agent    agent    TASK   BATTLE  ACHIEVEMENT   FAMILY
  |____|   |____|    VER    ACCOUNT ANTI CHEAT    OFFLINE
     |      |           
      \    /      LBS    SNS     RANK     OSS(IDIP)   LOG     Global   etc
       \  /_______|______|________|________|___________|_______|_______|__
        -
      [MysqlAgent] - lib
        |
      DBCacheProxy [group]
        |
        |------ Redis --- Redis -- Redis
        |
     DBInst (mysql / leveldb / ssdb)  ---------- OLAP 










	
