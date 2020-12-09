// Copyright Microsoft and Project Verona Contributors.
// SPDX-License-Identifier: MIT
#include "parser.h"
#include "pass.h"
#include "path.h"
#include "print.h"
#include "resolve.h"

#include <CLI/CLI.hpp>

constexpr auto stdlib = "stdlib";

int main(int argc, char** argv)
{
  using namespace verona::parser;

  CLI::App app{"Verona Parser"};
  std::string path;
  app.add_option("path", path, "Path to module to compile.")->required();

  try
  {
    app.parse(argc, argv);
  }
  catch (const CLI::ParseError& e)
  {
    return app.exit(e);
  }

  auto stdlibpath = path::canonical(path::join(path::executable(), stdlib));
  auto [ok, ast] = parse(path, stdlibpath);

  if (ok)
    ok = Resolve() << ast;

  std::cout << pretty(ast) << std::endl;
  return ok ? 0 : -1;
}
