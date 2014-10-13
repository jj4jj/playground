the data center is a local data agent .
it supports some functions as follow to help system manage data easily .
1. a share memory collections recoverable to support fast restart process
2. a remote public cache to share with others .
3. a remote public cache and floor it down to mysql db auto .
4. remote data in db , directly read and write .

the 4 functions can satisfied most requiration in playground .

eg. agent server fw .


Agent:
    init
	init	res
	init 	channel
	init	dataagent
    proc
        handle back end msg
	handle front end (gate) msg
        		
    tick (timer)
	handle agent timer
	handle submodle timer
    close
        close agents	
    destroy
	uninit datacenter
	uninit channel
 	uninit	res
usage:
	import DataCenter.h

	hLocal = DataCenter.RegisterData(LocalShmCache);
	hRemote = DataCenter.RegisterData(RemoteShmCache);
	hRemoteAndPersist = DataCenter.RegisterData(RemoteShmCacheAndPersistence);
	hPersist = DataCenter.RegisterData(Persistence);
	
	
	DataCenter.Init();
		LoadLocalShmCache(Recover)
		ConnectToRemoteCache
		ConnectToDB

	hColl =	hLocal.GetCollection(name);
	hColl.Get();
	hColl.Set();
	hColl = hRemote.GetCollection(name);
	hColl.Get();//asynchrous get
	hColl.OnGet();//if not important u can ignore it.
	hColl.Set();
	hColl.OnSet();//
	hColl = hRemoteCacheAndDB(name);
	hColL.Set();
	hColl.OnSet();
	hColl.Get(); 
	hCol.OnGet();
