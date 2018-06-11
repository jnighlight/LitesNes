#pragma once

class CpuInstruction
{
public:
	CpuInstruction();
	virtual ~CpuInstruction();

	virtual void Execute() = 0;
};
