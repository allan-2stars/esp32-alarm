import os

def load_env(env_path=".env"):
    if not os.path.exists(env_path):
        print(f"No .env found at {env_path}")
        return
    print("Loading environment from .env")
    with open(env_path) as f:
        for line in f:
            if "=" in line and not line.startswith("#"):
                key, value = line.strip().split("=", 1)
                os.environ[key] = value

load_env()
