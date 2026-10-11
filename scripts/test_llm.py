#!/usr/bin/env python3
import os
import urllib.request
import json

API_KEY = os.getenv("USTC_API_KEY", "sk-YBTk_IMCw-VFcePrVoh-rg")
BASE_URL = "https://api.llm.ustc.edu.cn/v1"
MODEL = "gpt-4o"

def test_ustc_llm():
    url = f"{BASE_URL}/chat/completions"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {API_KEY}"
    }
    payload = {
        "model": MODEL,
        "messages": [
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": "Hello! Please reply with 'USTC LLM is working correctly.' if you receive this."}
        ],
        "max_tokens": 100,
        "temperature": 0.1
    }

    print(f"Testing USTC LLM API...")
    print(f"URL: {url}")
    print(f"Model: {MODEL}")

    req = urllib.request.Request(
        url,
        data=json.dumps(payload).encode("utf-8"),
        headers=headers,
        method="POST"
    )

    try:
        with urllib.request.urlopen(req, timeout=30) as response:
            status_code = response.status
            body = response.read().decode("utf-8")
            print(f"\n[Success] Status Code: {status_code}")
            data = json.loads(body)
            reply = data["choices"][0]["message"]["content"]
            print(f"[AI Reply]: {reply}")
            return True
    except Exception as e:
        print(f"\n[Error] Failed to connect to USTC LLM API: {e}")
        return False

if __name__ == "__main__":
    test_ustc_llm()
