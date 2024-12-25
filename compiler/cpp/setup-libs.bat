@echo off
mkdir lib
cd lib
mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp" --output lib/json/json.hpp
git clone --depth 1 --branch v1.14.0 https://github.com/google/googletest.git