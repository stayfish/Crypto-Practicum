# 同济大学现代密码学课程设计

#### AES算法实现

1. 算法中的变量：
   
   * state是算法中执行操作的二维数组。该数组有四行，Nb列
   
   * Key是一个16bytes的字符串。最初的密钥是128位的种子密钥，由种子密钥生成共Nr+1轮子密钥，每一轮子密钥也是16bytes，用于和state中的每个分量进行异或运算。

2. 程序结构：
   
   | 文件名      | 功能                  |
   | -------- | ------------------- |
   | F256.h   | 声明域$F_{2^8}$ 接口的头文件 |
   | AES.h    | 声明AES操作接口的头文件       |
   | const.h  | 声明全局常数的头文件          |
   | F256.cpp | 实现域$F_{2^8}$ 上的相关操作 |
   | AES.cpp  | 实现AES加密的各项步骤        |
   | main.cpp | 测试主程序               |

3. 运行效果
   
   ![menu_aes](images/menu_aes.png)

4. 测试数据
   
   > message: 32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34
   > 
   > cipher key: 2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c

使用AES test进行测试，将message和cipher key分别写入文件 `fm.txt`  和 `fk.txt`  最终将加密结果输出到文件 `code_ch.txt`   和`code_hex.txt` 中

可以和`AES标准fips-197[1].PDF.pdf` 中测试数据进行对比。

#### CBC模式实现

在AES代码上添加功能即可

1. 程序结构
   
   | 文件名      | 功能                  |
   | -------- | ------------------- |
   | F256.h   | 声明域$F_{2^8}$ 接口的头文件 |
   | AES.h    | 声明AES操作接口的头文件       |
   | const.h  | 声明全局常数的头文件          |
   | CBC.h    | CBC模式加解密的接口         |
   | F256.cpp | 实现域$F_{2^8}$ 上的相关操作 |
   | AES.cpp  | 实现AES加密的各项步骤        |
   | CBC.cpp  | 实现CBC加解密的操作         |
   | main.cpp | 测试主程序               |

2. 运行效果

        ![menu_aes](images/menu_aes.png)

        其中后面四项功能是对ascii文件进行加密解密的

#### SHA-1算法

1. 涉及到的数据结构：

        `string`,`stl::bitset`,`stl::vector`
