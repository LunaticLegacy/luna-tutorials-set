## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本教程可能较为硬核，适合具有一定计算机基础和编程基础的人食用。如果在食用过程中昏厥或口吐白沫，本人概不负责。

## 目录

- [1. 介绍](#1-介绍)
- [2. 环境搭建](#2-环境搭建)
- [3. 基本语法](#3-基本语法)
- [4. 控制结构](#4-控制结构)
- [5. 函数](#5-函数)
- [6. 对象和数组](#6-对象和数组)
- [7. 异步编程](#7-异步编程)
- [8. ES6+新特性](#8-es6新特性)
- [9. 错误处理](#9-错误处理)
- [10. 模块化](#10-模块化)
- [11. 浏览器相关API](#11-浏览器相关api)
- [12. 实践项目](#12-实践项目)
- [13. 最佳实践](#13-最佳实践)
- [14. 总结](#14-总结)

## 1. 介绍

JavaScript是一种高级的、解释型的编程语言，它是一门基于原型和多范式的动态脚本语言。JavaScript支持面向对象编程、命令式编程和函数式编程等多种编程范式。

JavaScript最初被设计用于在浏览器中为网页添加动态交互功能，但现在已经被广泛应用于服务器端开发（Node.js）、移动应用开发、桌面应用开发等多个领域。

JavaScript与Java虽然名字相似，但它们是两种完全不同的编程语言，设计目的和语法结构都有很大差异。

## 2. 环境搭建

要运行JavaScript代码，你有以下几种选择：

1. 浏览器控制台：几乎所有现代浏览器都内置了JavaScript引擎，可以直接在开发者工具中运行JavaScript代码
2. Node.js：可以在本地安装Node.js环境来运行JavaScript代码
3. 在线编辑器：如CodePen、JSFiddle等在线平台

对于本教程，我们推荐使用浏览器控制台进行简单的测试和学习，使用Node.js进行更复杂的项目开发。

## 3. 基本语法

### 3.1 变量声明

JavaScript中有三种声明变量的方式：

1. `var`：传统的变量声明方式（ES5及以前）
2. `let`：块级作用域的变量声明方式（ES6新增）
3. `const`：声明常量的方式（ES6新增）

``js
var name = "张三";
let age = 25;
const PI = 3.14159;

// let和const具有块级作用域
{
  let innerVariable = "只能在块内访问";
  console.log(innerVariable); // 正常输出
}
// console.log(innerVariable); // 报错：innerVariable未定义
```

### 3.2 数据类型

JavaScript有以下几种基本数据类型：

1. Number（数字）
2. String（字符串）
3. Boolean（布尔值）
4. Undefined（未定义）
5. Null（空值）
6. Symbol（符号，ES6新增）
7. BigInt（大整数，ES2020新增）

以及复杂数据类型：

1. Object（对象）
2. Array（数组）
3. Function（函数）

``js
// 基本数据类型示例
let num = 42;                // Number
let str = "Hello World";     // String
let bool = true;             // Boolean
let empty = null;            // Null
let notDefined;              // Undefined

// 对象和数组
let person = {
  name: "李四",
  age: 30
};

let colors = ["红色", "绿色", "蓝色"]; // Array
```

### 3.3 运算符

JavaScript支持多种运算符：

1. 算术运算符：+、-、*、/、%、++、--
2. 比较运算符：==、===、!=、!==、>、<、>=、<=
3. 逻辑运算符：&&、||、!
4. 赋值运算符：=、+=、-=、*=、/= 等

``js
let a = 10;
let b = 5;

console.log(a + b);  // 15
console.log(a > b);  // true
console.log(a === 10 && b === 5);  // true

// 注意 == 和 === 的区别
console.log(5 == "5");   // true (类型转换)
console.log(5 === "5");  // false (严格相等)
```

## 4. 控制结构

### 4.1 条件语句

JavaScript支持if...else和switch语句：

``js
// if...else语句
let score = 85;

if (score >= 90) {
  console.log("优秀");
} else if (score >= 80) {
  console.log("良好");
} else if (score >= 60) {
  console.log("及格");
} else {
  console.log("不及格");
}

// switch语句
let day = 3;
switch (day) {
  case 1:
    console.log("星期一");
    break;
  case 2:
    console.log("星期二");
    break;
  case 3:
    console.log("星期三");
    break;
  default:
    console.log("其他");
}
```

### 4.2 循环语句

JavaScript支持多种循环结构：

``js
// for循环
for (let i = 0; i < 5; i++) {
  console.log("循环第" + (i+1) + "次");
}

// while循环
let count = 0;
while (count < 3) {
  console.log("while循环第" + (count+1) + "次");
  count++;
}

// do...while循环
let num = 0;
do {
  console.log("do...while循环第" + (num+1) + "次");
  num++;
} while (num < 3);

// for...in循环（遍历对象属性）
let obj = {a: 1, b: 2, c: 3};
for (let key in obj) {
  console.log(key + ": " + obj[key]);
}

// for...of循环（遍历可迭代对象）
let arr = [1, 2, 3, 4, 5];
for (let value of arr) {
  console.log(value);
}
```

## 5. 函数

函数是JavaScript中的一等公民，可以作为参数传递、作为返回值返回。

### 5.1 函数声明

``js
// 函数声明
function greet(name) {
  return "Hello, " + name + "!";
}

// 函数表达式
let greet2 = function(name) {
  return "Hi, " + name + "!";
};

// 箭头函数（ES6新增）
let greet3 = (name) => {
  return "Hey, " + name + "!";
};

// 箭头函数简写形式
let greet4 = name => "Hi there, " + name + "!";

console.log(greet("张三"));   // Hello, 张三!
console.log(greet2("李四"));  // Hi, 李四!
console.log(greet3("王五"));  // Hey, 王五!
console.log(greet4("赵六"));  // Hi there, 赵六!
```

### 5.2 参数处理

``js
// 默认参数（ES6新增）
function multiply(a, b = 1) {
  return a * b;
}

console.log(multiply(5));    // 5
console.log(multiply(5, 2)); // 10

// 剩余参数（ES6新增）
function sum(...numbers) {
  return numbers.reduce((total, num) => total + num, 0);
}

console.log(sum(1, 2, 3, 4, 5)); // 15

// 解构赋值参数
function printPerson({name, age}) {
  console.log("姓名：" + name + "，年龄：" + age);
}

let person = {name: "张三", age: 25};
printPerson(person); // 姓名：张三，年龄：25
```

## 6. 对象和数组

### 6.1 对象

JavaScript对象是属性和方法的集合：

``js
// 对象字面量
let person = {
  name: "张三",
  age: 25,
  gender: "男",
  
  // 方法
  greet: function() {
    return "你好，我是" + this.name;
  },
  
  // ES6简写方法
  sayAge() {
    return "我今年" + this.age + "岁";
  }
};

console.log(person.name);        // 张三
console.log(person["age"]);      // 25
console.log(person.greet());     // 你好，我是张三
console.log(person.sayAge());    // 我今年25岁

// 添加属性
person.job = "程序员";

// 删除属性
delete person.gender;
```

### 6.2 数组

JavaScript数组是有序的数据集合：

``js
// 创建数组
let fruits = ["苹果", "香蕉", "橙子"];
let numbers = new Array(1, 2, 3, 4, 5);

// 访问数组元素
console.log(fruits[0]);  // 苹果
console.log(fruits.length);  // 3

// 常用数组方法
fruits.push("葡萄");        // 在末尾添加元素
fruits.pop();              // 删除末尾元素
fruits.unshift("草莓");     // 在开头添加元素
fruits.shift();            // 删除开头元素

// 数组遍历
fruits.forEach(function(item, index) {
  console.log(index + ": " + item);
});

// 数组映射
let doubled = numbers.map(x => x * 2);
console.log(doubled);  // [2, 4, 6, 8, 10]

// 数组过滤
let evens = numbers.filter(x => x % 2 === 0);
console.log(evens);  // [2, 4]
```

## 7. 异步编程

JavaScript是单线程的，但通过异步编程可以处理耗时操作而不阻塞主线程。

### 7.1 回调函数

``js
// 模拟异步操作
function fetchData(callback) {
  setTimeout(() => {
    callback("数据获取成功");
  }, 1000);
}

fetchData(function(data) {
  console.log(data);  // 1秒后输出：数据获取成功
});
```

### 7.2 Promise（ES6新增）

``js
// 创建Promise
function fetchUserData() {
  return new Promise((resolve, reject) => {
    setTimeout(() => {
      let success = true;
      if (success) {
        resolve("用户数据");
      } else {
        reject("获取失败");
      }
    }, 1000);
  });
}

// 使用Promise
fetchUserData()
  .then(data => {
    console.log(data);  // 用户数据
  })
  .catch(error => {
    console.log(error); // 获取失败
  });
```

### 7.3 async/await（ES2017新增）

``js
// 使用async/await
async function getData() {
  try {
    let data = await fetchUserData();
    console.log(data);  // 用户数据
  } catch (error) {
    console.log(error); // 获取失败
  }
}

getData();
```

## 8. ES6+新特性

### 8.1 模板字符串

``js
let name = "张三";
let age = 25;

// ES5字符串拼接
let str1 = "我叫" + name + "，今年" + age + "岁";

// ES6模板字符串
let str2 = `我叫${name}，今年${age}岁`;

console.log(str1);  // 我叫张三，今年25岁
console.log(str2);  // 我叫张三，今年25岁
```

### 8.2 解构赋值

``js
// 数组解构
let [a, b, c] = [1, 2, 3];
console.log(a, b, c);  // 1 2 3

// 对象解构
let {name, age} = {name: "张三", age: 25};
console.log(name, age);  // 张三 25

// 重命名
let {name: userName, age: userAge} = {name: "李四", age: 30};
console.log(userName, userAge);  // 李四 30
```

### 8.3 类（ES6新增）

``js
// ES6类定义
class Person {
  constructor(name, age) {
    this.name = name;
    this.age = age;
  }
  
  greet() {
    return `你好，我是${this.name}`;
  }
  
  // 静态方法
  static getType() {
    return "人类";
  }
}

// 继承
class Student extends Person {
  constructor(name, age, grade) {
    super(name, age);
    this.grade = grade;
  }
  
  study() {
    return `${this.name}正在学习`;
  }
}

let student = new Student("张三", 20, "大一");
console.log(student.greet());     // 你好，我是张三
console.log(student.study());     // 张三正在学习
console.log(Student.getType());   // 人类
```

## 9. 错误处理

JavaScript通过try...catch语句处理运行时错误：

``js
try {
  // 可能出错的代码
  let result = riskyOperation();
  console.log(result);
} catch (error) {
  // 错误处理
  console.log("发生错误：" + error.message);
} finally {
  // 无论是否出错都会执行
  console.log("清理工作");
}

// 自定义错误
function validateAge(age) {
  if (age < 0) {
    throw new Error("年龄不能为负数");
  }
  return true;
}

try {
  validateAge(-5);
} catch (error) {
  console.log(error.message);  // 年龄不能为负数
}
```

## 10. 模块化（ES6新增）

JavaScript ES6引入了模块系统：

``js
// math.js - 导出模块
export function add(a, b) {
  return a + b;
}

export function subtract(a, b) {
  return a - b;
}

// 默认导出
export default function multiply(a, b) {
  return a * b;
}

// main.js - 导入模块
import multiply, { add, subtract } from './math.js';

console.log(add(2, 3));       // 5
console.log(subtract(5, 2));  // 3
console.log(multiply(3, 4));  // 12

// 导入所有
import * as math from './math.js';
console.log(math.add(1, 2));  // 3
```

## 11. 浏览器相关API

JavaScript在浏览器环境中提供了丰富的API，使我们能够操作网页元素、处理用户交互、发送网络请求等。

### 11.1 DOM操作

DOM（Document Object Model）是浏览器提供的用于操作HTML文档的API。

``js
// 获取元素
const element = document.getElementById("myId");
const elements = document.getElementsByClassName("myClass");
const queryElement = document.querySelector(".myClass");
const queryElements = document.querySelectorAll("p");

// 创建元素
const newDiv = document.createElement("div");
newDiv.textContent = "Hello World";
document.body.appendChild(newDiv);

// 修改元素
element.style.color = "red";
element.className = "newClass";
element.setAttribute("data-id", "123");

// 删除元素
element.remove();
```

### 11.2 事件处理

浏览器提供事件系统来处理用户交互：

``js
// 添加事件监听器
const button = document.querySelector("button");
button.addEventListener("click", function(event) {
  console.log("按钮被点击了");
});

// 事件对象
button.addEventListener("click", function(event) {
  console.log("鼠标位置：", event.clientX, event.clientY);
});

// 事件冒泡与捕获
document.body.addEventListener("click", function() {
  console.log("body被点击");
}, true); // true表示在捕获阶段触发

// 移除事件监听器
const handleClick = function() {
  console.log("点击事件");
};
button.addEventListener("click", handleClick);
button.removeEventListener("click", handleClick);
```

### 11.3 网络请求

使用Fetch API进行网络请求：

``js
// GET请求
fetch("https://api.example.com/data")
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error("Error:", error));

// POST请求
fetch("https://api.example.com/users", {
  method: "POST",
  headers: {
    "Content-Type": "application/json"
  },
  body: JSON.stringify({
    name: "张三",
    age: 25
  })
})
  .then(response => response.json())
  .then(data => console.log(data));

// 使用async/await
async function fetchData() {
  try {
    const response = await fetch("https://api.example.com/data");
    const data = await response.json();
    console.log(data);
  } catch (error) {
    console.error("Error:", error);
  }
}
```

### 11.4 存储API

浏览器提供了多种存储数据的方式：

``js
// localStorage - 持久化存储
localStorage.setItem("username", "张三");
const username = localStorage.getItem("username");
localStorage.removeItem("username");
localStorage.clear(); // 清空所有数据

// sessionStorage - 会话存储（关闭浏览器标签页后清除）
sessionStorage.setItem("tempData", "临时数据");
const tempData = sessionStorage.getItem("tempData");

// Cookie操作
document.cookie = "username=张三; expires=Thu, 18 Dec 2025 12:00:00 UTC; path=/";
```

### 11.5 其他常用浏览器API

``js
// 定时器
const timerId = setTimeout(() => {
  console.log("延迟执行");
}, 1000);

const intervalId = setInterval(() => {
  console.log("周期执行");
}, 2000);

// 清除定时器
clearTimeout(timerId);
clearInterval(intervalId);

// 位置API（需要用户授权）
navigator.geolocation.getCurrentPosition(
  position => {
    console.log("纬度：" + position.coords.latitude);
    console.log("经度：" + position.coords.longitude);
  },
  error => {
    console.error("获取位置失败：" + error.message);
  }
);

// History API
history.pushState({page: 1}, "标题", "/page1");
history.replaceState({page: 2}, "标题", "/page2");
window.addEventListener("popstate", function(event) {
  console.log("页面状态变更", event.state);
});
```

## 12. 实践项目

让我们通过一个简单的待办事项应用来实践所学知识：

``js
class TodoApp {
  constructor(containerId) {
    this.container = document.getElementById(containerId);
    this.todos = JSON.parse(localStorage.getItem("todos")) || [];
    this.idCounter = this.todos.length > 0 ? 
      Math.max(...this.todos.map(t => t.id)) + 1 : 1;
    this.init();
  }
  
  init() {
    this.render();
    this.bindEvents();
  }
  
  addTodo(text) {
    if (!text.trim()) return;
    
    const todo = {
      id: this.idCounter++,
      text: text.trim(),
      completed: false,
      createdAt: new Date()
    };
    
    this.todos.push(todo);
    this.saveToStorage();
    this.render();
  }
  
  removeTodo(id) {
    this.todos = this.todos.filter(todo => todo.id !== id);
    this.saveToStorage();
    this.render();
  }
  
  toggleTodo(id) {
    const todo = this.todos.find(todo => todo.id === id);
    if (todo) {
      todo.completed = !todo.completed;
      this.saveToStorage();
      this.render();
    }
  }
  
  saveToStorage() {
    localStorage.setItem("todos", JSON.stringify(this.todos));
  }
  
  render() {
    const todoList = this.todos.map(todo => `
      <li class="${todo.completed ? 'completed' : ''}" data-id="${todo.id}">
        <input type="checkbox" ${todo.completed ? 'checked' : ''}>
        <span>${todo.text}</span>
        <button class="delete-btn">删除</button>
      </li>
    `).join('');
    
    this.container.innerHTML = `
      <div class="todo-app">
        <h2>待办事项</h2>
        <div class="add-todo">
          <input type="text" id="new-todo" placeholder="添加新任务...">
          <button id="add-btn">添加</button>
        </div>
        <ul class="todo-list">
          ${todoList}
        </ul>
        <div class="stats">
          <span>总计: ${this.todos.length} | </span>
          <span>已完成: ${this.todos.filter(t => t.completed).length}</span>
        </div>
      </div>
    `;
  }
  
  bindEvents() {
    // 重新绑定事件，因为每次render后DOM都会重新创建
    this.container.addEventListener('click', (e) => {
      if (e.target.id === 'add-btn') {
        const input = this.container.querySelector('#new-todo');
        this.addTodo(input.value);
        input.value = '';
      } else if (e.target.type === 'checkbox') {
        const li = e.target.closest('li');
        this.toggleTodo(Number(li.dataset.id));
      } else if (e.target.classList.contains('delete-btn')) {
        const li = e.target.closest('li');
        this.removeTodo(Number(li.dataset.id));
      }
    });
    
    this.container.addEventListener('keypress', (e) => {
      if (e.target.id === 'new-todo' && e.key === 'Enter') {
        this.addTodo(e.target.value);
        e.target.value = '';
      }
    });
  }
}

// 使用示例（需要HTML中有对应id的元素）
// const app = new TodoApp("todo-container");
```

## 13. 最佳实践

1. 使用const和let代替var
2. 使用箭头函数处理this绑定问题
3. 使用模板字符串代替字符串拼接
4. 使用解构赋值简化代码
5. 使用扩展运算符(...)处理数组和对象
6. 使用Promise或async/await处理异步操作
7. 使用ES6模块组织代码
8. 使用严格模式("use strict")

``js
// 好的实践示例
const processData = (data) => {
  "use strict";
  
  const {name, values} = data;
  const processedValues = [...values].map(val => val * 2);
  
  return {
    name,
    processedValues,
    total: processedValues.reduce((sum, val) => sum + val, 0)
  };
};
```

## 14. 总结

JavaScript是一门功能强大且灵活的编程语言。通过本教程，我们学习了：

1. JavaScript的基本语法和数据类型
2. 控制结构和函数
3. 对象和数组操作
4. 异步编程
5. ES6+新特性
6. 浏览器API的使用
7. 错误处理和模块化
8. 实践项目开发

掌握这些基础知识后，你可以继续深入学习：
- 更高级的JavaScript概念（闭包、原型链等）
- 前端框架（React、Vue、Angular等）
- Node.js后端开发
- 测试驱动开发
- 性能优化技巧

持续练习和实践是掌握JavaScript的关键。祝你在JavaScript学习旅程中取得成功！
