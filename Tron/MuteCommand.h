#pragma once
#include <Command.h>

namespace dae
{

    class MuteCommand : public Command
    {
      public:
        MuteCommand() = default;
        virtual ~MuteCommand() = default;

        virtual void Execute() override;

    };
}

