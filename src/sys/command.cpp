// ======================================================================== //
// Copyright (C) 2011 Benjamin Segovia                                      //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "command.hpp"
#include "script.hpp"
#include <sstream>
#include <cstring>

namespace pf
{
  ConVarSystem *ConVarSystem::global = NULL;

  ConVarSystem::ConVarSystem(void) : modified(false) {}
  ConVarSystem::~ConVarSystem(void) {}

  ConVar &ConVarSystem::get(size_t index) {
    PF_ASSERT(index < var.size());
    return var[index];
  }
  const ConVar &ConVarSystem::get(size_t index) const {
    PF_ASSERT(index < var.size());
    return var[index];
  }

  ConVarSystem *ConVarSystem::clone(void) const {
    ConVarSystem *clone = PF_NEW(ConVarSystem);
    clone->var = this->var;
    clone->modified = false;
    return clone;
  }

  ConVar::ConVar(const char *name, int32 min, int32 curr, int32 max, const char *desc)
  {
    if (ConVarSystem::global == NULL) ConVarSystem::global = new ConVarSystem;
    this->index = ConVarSystem::global->var.size();
    this->type = CVAR_INT;
    this->i = curr;
    this->imin = min;
    this->imax = max;
    this->name = name;
    this->desc = desc;
    ConVarSystem::global->var.push_back(*this);
  }

  ConVar::ConVar(const char *name, float min, float curr, float max, const char *desc)
  {
    if (ConVarSystem::global == NULL) ConVarSystem::global = new ConVarSystem;
    this->index = ConVarSystem::global->var.size();
    this->type = CVAR_FLOAT;
    this->f = curr;
    this->fmin = min;
    this->fmax = max;
    this->name = name;
    this->desc = desc;
    ConVarSystem::global->var.push_back(*this);
  }

  ConVar::ConVar(const char *name, const std::string &str, const char *desc)
  {
    if (ConVarSystem::global == NULL) ConVarSystem::global = new ConVarSystem;
    this->index = ConVarSystem::global->var.size();
    this->type = CVAR_STRING;
    this->str = str;
    this->name = name;
    this->desc = desc;
    ConVarSystem::global->var.push_back(*this);
  }

  vector<ConCommand> *ConCommand::cmds = NULL;

  ConCommand::ConCommand(const char *name, const char *argument, char ret) :
    name(name), argument(argument), ret(ret)
  {
    PF_ASSERT(name != NULL && argument != NULL);
    PF_ASSERT(ret == 'i' || ret == 'f' || ret == 0);
    if (UNLIKELY(ConCommand::cmds == NULL))
      ConCommand::cmds = PF_NEW(vector<ConCommand>);
    ConCommand::cmds->push_back(*this);
  }

  void ConsoleSystemStart(ScriptSystem &scriptSystem)
  {
    if (ConCommand::cmds == NULL)
      return;
    else {
      std::stringstream ss;
      for (size_t cmdID = 0; cmdID < ConCommand::cmds->size(); ++cmdID) {
        ConCommand &cmd = (*ConCommand::cmds)[cmdID];
        ss << "local ffi = require \"ffi\"\n";
        ss << "ffi.cdef[[";
        switch (cmd.ret) {
          case  0 : ss << "void "; break;
          case 'i': ss << "int32_t "; break;
          case 'f': ss << "float  "; break;
          default: FATAL("Unsupported returned value type");
        }
        ss << cmd.name << "(";
        const size_t argNum = strlen(cmd.argument);
        for (size_t i = 0; i < argNum; ++i) {
          const char arg = cmd.argument[i];
          switch (arg) {
            case 'i': ss << "int32_t "; break;
            case 'f': ss << "float  "; break;
            case 's': ss << "const char *"; break;
            default: FATAL("Unsupported argument type");
          }
          if (i+1 != argNum) ss << ", ";
        }
        ss << ");]]\n";
      }

      // Upload the code to the Lua State
      ScriptStatus status;
      const std::string luaCode = ss.str();
      const char *str = luaCode.c_str();
      scriptSystem.runNonProtected(str, status);
      if (UNLIKELY(!status.success))
        FATAL(std::string("Failed to initialize console system: ") + status.msg);
    }
  }

  void ConsoleSystemEnd(void)
  {
    ConVarSystem::global = NULL; //!< Free the console variable array
    delete ConCommand::cmds;     //!< Free the console commands
    ConCommand::cmds = NULL;
  }

} /* namespace pf */
