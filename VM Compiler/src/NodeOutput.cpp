#include "Node.h"
#include "parser.hpp"

void Node::OutputMargin(std::ostream& stream, int depth) const
{
	for (int i = 0; i < depth; i++)
	{
		stream << "--";
	}
}

void NBlock::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Block\n";
	for (auto& stmt : mStatements)
	{
		stmt->OutputAST(stream, depth + 1);
		stream.flush();
	}
}

void NData::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Data\n";
	for (auto& decl : mDecls)
	{
		decl->OutputAST(stream, depth + 1);
	}
}

void NProgram::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Program\n";
	mData->OutputAST(stream, depth + 1);
	mMain->OutputAST(stream, depth + 1);
}

void NNumeric::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Numeric: " << mValue << '\n';
}

void NVarDecl::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "VarDecl: " << mName << '\n';
}

void NArrayDecl::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "ArrayDecl: " << mName << '\n';
	mSize->OutputAST(stream, depth + 1);
}

void NNumericExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "NumericExpr\n";
	mNumeric->OutputAST(stream, depth + 1);
}

void NVarExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "VarExpr: " << mName << '\n';
}

void NBinaryExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	
	char op = ' ';
	switch (mType)
	{
	case TADD:
		op = '+';
		break;
	case TSUB:
		op = '-';
		break;
	case TMUL:
		op = '*';
		break;
	case TDIV:
		op = '/';
		break;
	default:
		break;
	}

	stream << "BinaryExpr: " << op << '\n';
	mLhs->OutputAST(stream, depth + 1);
	mRhs->OutputAST(stream, depth + 1);
}

void NArrayExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "ArrayExpr: " << mName << '\n';
	mSubscript->OutputAST(stream, depth + 1);
}

void NAssignVarStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "AssignVarStmt: " << mName << '\n';
	mRhs->OutputAST(stream, depth + 1);
}

void NAssignArrayStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "AssignArrayStmt: " << mName << '\n';
	mSubscript->OutputAST(stream, depth + 1);
	mRhs->OutputAST(stream, depth + 1);
}

void NIncStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "IncStmt: " << mName << '\n';
}

void NDecStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "DecStmt: " << mName << '\n';
}

void NComparison::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);

	std::string op;
	switch (mType)
	{
	case TLESS:
		op = "<";
		break;
	case TISEQUAL:
		op = "==";
		break;
	default:
		break;
	}

	stream << "Comparison: " << op << '\n';
	mLhs->OutputAST(stream, depth + 1);
	mRhs->OutputAST(stream, depth + 1);
}

void NIfStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);

	if (mElseBlock == nullptr)
	{
		stream << "IfStmt (no else)\n";
	}
	else
	{
		stream << "IfStmt (with else)\n";
	}
	
	mComp->OutputAST(stream, depth + 1);
	mIfBlock->OutputAST(stream, depth + 1);
	if (mElseBlock != nullptr)
	{
		mElseBlock->OutputAST(stream, depth + 1);
	}
}

void NWhileStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "WhileStmt\n";
	mComp->OutputAST(stream, depth + 1);
	mBlock->OutputAST(stream, depth + 1);
}

void NPenUpStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "PenUpStmt\n";
}

void NPenDownStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "PenDownStmt\n";
}

void NSetPosStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "SetPosStmt\n";
	mXExpr->OutputAST(stream, depth + 1);
	mYExpr->OutputAST(stream, depth + 1);
}

void NSetColorStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "SetColorStmt\n";
	mColor->OutputAST(stream, depth + 1);
}

void NFwdStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "FwdStmt\n";
	mParam->OutputAST(stream, depth + 1);
}

void NBackStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "BackStmt\n";
	mParam->OutputAST(stream, depth + 1);
}

void NRotStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "RotStmt\n";
	mParam->OutputAST(stream, depth + 1);
}
