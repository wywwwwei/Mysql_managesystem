# Pharmaceutical Sales Management System

[TOC]

## 用户登陆
>默认**管理员**
>username:  **admin**   password:   **admin**

点击LOGIN之后不可再编辑username/password，点击LOGOUT之后可重新输入登陆

## 权限管理
>+ 对应不同的用户，所显示的能操作的表也不一样，这取决于你的权限
>+ 相应的，在对应的表的操作中，会根据你的权限限制你的**读/写/删**操作

## 执行添加操作
先点击**add item**，然后表格会新增空行，并且出现**add item ok**的按钮，只需要在新增空行输入所需插入的数据后点击**add item ok**即可.
输入数据**并不需要输入所有列**，基本只需要将**与id相关的数据输入即可，其他的数据会根据该ID到其他表查询并完善**
>+ 对于不同的表，添加操作所能执行的功能是不一致的

### warehousee表
在warehouse中，因为我们的库存经常需要**更新(Update)**，所以在add item的时候会**先判断能否更新**(update ... set ... where...)，**再考虑插入**(insert into ... values(...))

### 非warehouse表
在非warehouse表中，一般是不需要更新的，所以**只有插入的功能**

## 执行删除操作
>选中行，并点击**delete item**即可

## FILTER
>**通过在输入框中输入过滤条件后点击Search即可筛选，FILTER格式：
>对应列的名称 [=<>] [比较的值]
>example
>columnname = 1 and columnname <> 1**
