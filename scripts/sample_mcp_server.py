#!/usr/bin/env python3
import sys
import json
import datetime

def handle_request(req):
    method = req.get("method")
    req_id = req.get("id")

    if method == "tools/list":
        return {
            "jsonrpc": "2.0",
            "id": req_id,
            "result": {
                "tools": [
                    {
                        "name": "get_system_time",
                        "description": "Get current system time and environment info.",
                        "inputSchema": {"type": "object", "properties": {}}
                    },
                    {
                        "name": "calculate",
                        "description": "Perform basic arithmetic calculation. Arguments: {'expr': '2 + 2'}",
                        "inputSchema": {
                            "type": "object",
                            "properties": {
                                "expr": {"type": "string"}
                            },
                            "required": ["expr"]
                        }
                    }
                ]
            }
        }
    elif method == "tools/call":
        params = req.get("params", {})
        tool_name = params.get("name")
        args = params.get("arguments", {})

        if tool_name == "get_system_time":
            return {
                "jsonrpc": "2.0",
                "id": req_id,
                "result": {
                    "success": True,
                    "content": f"Current system time: {datetime.datetime.now().isoformat()}"
                }
            }
        elif tool_name == "calculate":
            expr = args.get("expr", "0")
            try:
                # Safe evaluation of simple math expression
                res = eval(expr, {"__builtins__": {}}, {})
                return {
                    "jsonrpc": "2.0",
                    "id": req_id,
                    "result": {
                        "success": True,
                        "result": res
                    }
                }
            except Exception as e:
                return {
                    "jsonrpc": "2.0",
                    "id": req_id,
                    "error": {"code": -32603, "message": str(e)}
                }
    
    return {
        "jsonrpc": "2.0",
        "id": req_id,
        "error": {"code": -32601, "message": f"Method not found: {method}"}
    }

def main():
    line = sys.stdin.read()
    if not line.strip():
        return
    try:
        req = json.loads(line)
        resp = handle_request(req)
        print(json.dumps(resp))
    except Exception as e:
        err_resp = {
            "jsonrpc": "2.0",
            "id": None,
            "error": {"code": -32700, "message": f"Parse error: {str(e)}"}
        }
        print(json.dumps(err_resp))

if __name__ == "__main__":
    main()
