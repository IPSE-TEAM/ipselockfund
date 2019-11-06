### 锁仓矿池奖励计划

![](https://upload.cc/i1/2019/11/05/YSdVZO.jpeg)

### POST矿池抵押机制和解锁机制

![](https://upload.cc/i1/2019/11/05/OXMY7w.jpeg)

### 操作

想进行锁仓奖励的用户，将POST转账到账号`ipselockfund`，`memo`信息可填入自己名字，方便别人推荐，单次转账数量要满足条件：

	- 普通节点，需转500000 POST。
	- B级节点，需转2000000 POST。
	- A级节点，需转8000000 POST。

转入后，就完成了锁仓。

然后到解锁期开始后，每天都会根据浮动利率返相应的Token到用户账号。

### 权限

cleos set account permission ipselockfund active permission.json -p ipselockfund@active

cleos set account permission test active --add-code

