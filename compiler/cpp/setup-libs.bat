@echo off
cd lib
git clone https://github.com/nlohmann/json.git
curl "https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp" --output json/json.hpp