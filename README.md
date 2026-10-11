# Pilot — Multi-Agent Code Development System (多智能体编程协作系统)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.wikipedia.org/wiki/C++17)

> **声明 / Notice**
> 本项目是一个**全开源**的多智能体（Multi-Agent）协作开发系统，旨在为所有对智能体开发、C++ 高性能系统编程以及大模型应用感兴趣的开发者和学习者提供参考与研究。
> 欢迎大家共同学习、交流与贡献代码！
> 
> **联系作者 / Contact**
> 如果您在学习或使用本项目时遇到任何问题，欢迎通过邮箱与我联系：
> 📧 **li2542369686@163.com**
> 收到邮件后我会第一时间回复您。

---

## 🏗️ 架构特点 (Architecture Highlights)

Pilot 采用模块化、面向对象的设计理念，确保代码清晰、易于扩展，非常适合二次开发和团队协作：

1. **高性能核心 (Core Framework)**：
   - **Orchestrator**：中心调度器，负责任务编排、依赖关系管理及工作流调度。
   - **Message Bus**：基于发布-订阅模式的消息总线，实现 Agent 之间的低延迟异步通信。
   - **Context**：线程安全的共享上下文与键值内存存储。
   - **Agent / Task**：高度抽象的智能体基类与任务生命周期管理。

2. **多模型适配层 (LLM Adapter Layer)**：
   - 提供统一的 `ILLMClient` 接口与工厂模式。
   - 完美支持 OpenAI、Anthropic Claude、本地 Ollama 以及各类兼容 OpenAI 协议的自定义中转 API（如代理/自定义大模型端点）。

3. **工具插件化架构 (Tool Registry)**：
   - 提供模块化的 `ITool` 接口和全局 `ToolRegistry`，开发者可以极其轻松地为 Agent 编写和注册新工具（如文件读写、编译检查、Git 操作等）。

4. **交互式命令行 (Interactive CLI / REPL)**：
   - 友好的终端交互界面，支持实时查看多智能体任务执行过程与指令交互。

---

## 📁 目录结构 (Directory Structure)

```
Pilot/
├── src/
│   ├── core/               # 核心框架 (Orchestrator, Agent, MessageBus, Task, Context)
│   ├── llm/                # LLM 客户端与多模型适配工厂
│   ├── tools/              # 工具层与插件注册 (ITool & ToolRegistry)
│   ├── cli/                # 交互式命令行与 REPL 界面
│   └── main.cpp            # 程序主入口
├── config/
│   └── config.yaml         # 系统与模型配置文件
├── scripts/
│   ├── setup.sh            # 自动化编译安装脚本
│   ├── run.sh              # 快速运行脚本
│   └── llm_bridge.py       # 大模型 API 桥接与流式解析工具
├── CMakeLists.txt          # CMake 现代构建配置
├── .gitignore              # Git 忽略配置
└── README.md               # 项目说明文档
```

---

## 🚀 快速开始 (Quick Start)

### 1. 环境准备
确保您的系统已安装：
- **CMake** (>= 3.14)
- **GCC / Clang** (支持 C++17)
- **Python 3** (用于 LLM 桥接解析)

### 2. 编译项目
```bash
./scripts/setup.sh
```

### 3. 运行交互式 CLI
```bash
./build/pilot
```
在交互式界面中，您可以输入您的开发需求（例如：`编写一个线程安全的 C++17 内存缓存队列`），多智能体团队（Planner 与 Coder）将协同为您规划并实现代码。

或者直接通过命令行运行单次任务：
```bash
./scripts/run.sh "用 C++17 开发一个高性能 HTTP 服务器"
```

---

## 🤝 参与贡献 (Contributing)

我们非常欢迎全球的开发者和学习者参与到 Pilot 的共建中来！
1. Fork 本仓库
2. 创建您的特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交您的修改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启一个 Pull Request

如有任何疑问或建议，欢迎随时发送邮件至 **li2542369686@163.com** 交流讨论！
