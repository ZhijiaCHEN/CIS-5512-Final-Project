# mmaster usage

This program relies on mysqlclient shared library. You will need to export the LD_LIBRARY_PATH to run the program since the mysqlclient library is not placed in system's shared library path. You can use the following command: 

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:path/to/libmysqlclient`

For example, I placed the library at the path: `/home/zhijia/git/assignments/Operating\ Systems/lab4/php/mysql-connector/lib`, so the export command for me is:

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhijia/git/assignments/Operating\ Systems/lab4/php/mysql-connector/lib`

__Note__: To run mmaster in our tsh shell, please execute the export command before you start the tsh (if you ask the shell to run the mmaster in another terminal session, the shell would not know where the library is, as the export command is only effective for each session).

And also run the updated [final.sql](https://github.com/ZhijiaCHEN/CIS-5512-Final-Project/blob/master/final/sql/final.sql) in your database.

The mmaster asks for 7 arguments: 

1. tsport

    Your tsh listening port.

2. granularity
    
    Granularity for the distribution of matrix computation tasks.

3. P
    
    Number of mworker processes that compute the matrix.

4. SERVER

    Your MySQL database server.

5. USER

    As the name suggests.

6. PASSWORD

    As the name suggests.

7. DATABASE

    As the name suggests.

For example:

`./mmaster 9251 200 10 cis-linux2.temple.edu tuh17884 reeshaep fa18_5512_tuh17884`