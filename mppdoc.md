# MCFunction Plus Plus 语言规范
### 目录
1. MPP入门
    - 欢迎使用MPP！
    - MPP能做什么？
    - MPP基础语法
        - 什么是代码块
        - MPP与CMD
        - 计分板选择器 
2. MPP高级功能
    - xxx
    - xxx

# MPP入门
## 欢迎使用MPP！
MCFunction Plus Plus，  
重新定义MCFunction。  
由 @Messy_Ghost 倾情奉献
##### 更新日期：2020.7.11 v0.0.2

## MPP能做什么？
当动辄几个函数、计分板，  
只为了实现一个循环；  
想传参，难于上青天，  
这严重地阻滞了MCF在各个领域的发展。  
因此，我们发明了MPP。  
MPP致力于简化函数的编写，  
并赋予它高级语言的特征，  
使开发者能将精力集中于算法的设计，  
而不是在原始的环境中焦头烂额。  

## MPP基础语法
### 什么是代码块
我们谓MPP中的一段代码为“代码块”，  
与MCFunction中的“函数”概念相独立。  
代码块的一般定义如下：  
~~~
func <代码块名称>[<代码块类型>]([代码块参数1,2…]){
    #Do something fancy there
}
~~~
其中， `<代码块名称>` 应是有效的函数名称，  
 `<代码块类型>` 有两种取值， `MPP` 和 `CMD` ，   
  `[代码块参数1,2…]` 是可选的，将在后面的章节讨论。

### MPP与CMD
 `MPP` 与 `CMD` 两种取值有什么区别呢？  
 为了便于组织，MPP中所有代码块全部放置在 `.mpp` 文件里，编译时 `CMD` 块直接转化为普通的函数， `MPP` 将使该代码块作为MPP语言编译为函数。  
 我们现在可以写一个示例的代码块了：
```
func foo [CMD](){
    say 1
}
```
编译将生成 `foo.mcfunction` 函数，  
函数内容为 `say 1` 。

### 计分板选择器  
说了这么多，我们还是没有接触到MPP的代码，  
那就来了解一下吧！  
```
scb@Alex ++;
```
可能看上去有点难以理解，没关系，一起来分析一下：  
它的标准格式为： `<计分板名称>@<实体>` 。  
实体可以(且 __优先__ )使用匹配的玩家名，  
当然，你也可以使用选择器： `scb@e[type=area_effect_cloud]` ，该选择器会选择所有 `@e[type=area_effect_cloud]` 的 `scb` 分数。  
因此，上面的 `scb@Alex` 会选择玩家Alex的scb分数。  
 `++` 是自增运算符，上述操作即为把玩家Alex的scb分数自增1。  
 如果我们还想输出scb该怎么办呢？
 ```
 say scb@Alex;
 ```
 就是这么简单，将计分板变量直接输出！  
 MPP会自动将它解析为原始Json字符串。  
 到这里，你已经成功入门MPP了，  
 放轻松，编译一下这个代码块，或者继续学习高级语法吧！

## MPP高级功能
### 内联命令
MPP支持在MPP代码块内直接调用命令，  
格式为
```
[cmd]{
    say 1
    say MPP Yes! 
    #More cool stuff there
}
```
如果不想写多行命令，也可以直接
```
scb@Alex ++;
/say Scoreboard scb of Alex has changed
scb@Alex --;
```
 `/<命令>` 是在MPP代码块中调用命令的简便方式，  
 但是不推荐使用，而且它可能会引起混淆。

 ### 操作符
除了自增，计分板还可以进行别的操作吗？  
当然能！  
目前可用的操作符有： `= + - * / | & ^` 以及对应的合并运算符 `++ -- += -= *= /= |= &= ^=` 。比如：
```
scb@Steve = scb@Alex & scb@Steve;
```
它等效于
```
scb@Steve &= scb@Alex;
```

### 字符串
在MPP中，同样可以很方便地操作字符串，  
你可以混合使用计分板选择器、字符串和Json原始字符串。  
在MPP中表示Json原始字符串的方法： `<["Messy_Ghost","and","ChapterII"]>`   
混合使用如下：
```
say "MPP Version" + ver@Alex + <["by",{"color":"red","text":"Messy_Ghost and ChapterII"}]>;
```
MPP会自动连接字符串、计分板变量、Json原始字符串以及NBT标签。

### 循环和判断
作为一个~~高级~~语言，MPP支持循环等基础逻辑操作。
```
for(i@Alex;i@Alex < 10;i@Alex++){
    #Some hacky code
}

while(i@Alex != 10){
    say i@Alex;
    i@Alex ++;
}
```
语法大多数类似于标准C++，但要注意， __不能省略__ 计分板后的实体，一定要使用 `i@Alex ` 。  
当然，MPP也支持if-else语句，
```
if(i@Alex != 1){
    say "It isn't 1";
}
```

### 参数
还记得代码块参数吗？  
```
func foo [MPP](arg1,arg2){
    scb@Alex = arg1 + arg2;
}
```
参数只要简单地当作变量使用即可。  
__注意：此处的参数是按值传递！这意味着不能修改参数的值（可以修改，但是没有意义）__  
CMD式代码块不支持参数。

### ...And more cool stuff?

