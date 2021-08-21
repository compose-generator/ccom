@echo off
mkdir lib
cd lib
mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.10.0/json.hpp" --output lib/json/json.hpp
git clone https://github.com/google/googletest.git