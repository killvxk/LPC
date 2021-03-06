#pragma once
#include "CommonDefs.h"


typedef BOOLEAN BOOL;
typedef unsigned short WORD; 
typedef unsigned char BYTE;
typedef DWORD * PDWORD;
#define _Out_
#define _In_
#define _In_opt_

typedef enum WIN_VER_DETAIL{
	WINDOWS_VERSION_NONE,
	WINDOWS_VERSION_2K,
	WINDOWS_VERSION_XP,
	WINDOWS_VERSION_2K3,
	WINDOWS_VERSION_2K3_SP1_SP2,
	WINDOWS_VERSION_VISTA_2008,
	WINDOWS_VERSION_7_7600_UP,
	WINDOWS_VERSION_7_7000
} WIN_VER_DETAIL;

WIN_VER_DETAIL WinVersion;

typedef NTSTATUS (*PSPTERMINATETHREADBYPOINTER_K)(PETHREAD,NTSTATUS,BOOLEAN);
PSPTERMINATETHREADBYPOINTER_K PspTerminateThreadByPointer_K;
typedef NTSTATUS (*PSPTERMINATETHREADBYPOINTER_XP)(PETHREAD,NTSTATUS);
PSPTERMINATETHREADBYPOINTER_XP PspTerminateThreadByPointer_XP;

ULONG ThreadProc,ThreadListHead,ActiveProcessLinksOffset,UniqueProcessIdOffset,processpid;

typedef enum _KAPC_ENVIRONMENT
{
	OriginalApcEnvironment,
	AttachedApcEnvironment,
	CurrentApcEnvironment,
	InsertApcEnvironment
} KAPC_ENVIRONMENT;

NTKERNELAPI
VOID
KeInitializeApc(PKAPC Apc,
				PETHREAD Thread,
				KAPC_ENVIRONMENT Environment,
				PKKERNEL_ROUTINE KernelRoutine,
				PKRUNDOWN_ROUTINE RundownRoutine,
				PKNORMAL_ROUTINE NormalRoutine,
				KPROCESSOR_MODE ProcessorMode,
				PVOID NormelContext);

NTKERNELAPI
BOOLEAN
KeInsertQueueApc(PKAPC Apc,
				 PVOID SystemArgument1,
				 PVOID SystemArgument2,
				 KPRIORITY Increment);

#define PS_CROSS_THREAD_FLAGS_SYSTEM 0x00000010UL

#pragma pack(push,1)
typedef enum VALIDITY_CHECK_STATUS
{
	VCS_INVALID,
	VCS_VALID,
	VCS_TRANSITION,
	VCS_PAGEDOUT,
	VCS_DEMANDZERO,
	VCS_PROTOTYPE
} VALIDITY_CHECK_STATUS;

#define PAE_ON (1<<5)

typedef struct _MMPTE_SOFTWARE {
	ULONG Valid : 1;
	ULONG PageFileLow : 4;
	ULONG Protection : 5;
	ULONG Prototype : 1;
	ULONG Transition : 1;
	ULONG PageFileHigh : 20;
} MMPTE_SOFTWARE;

typedef struct _MMPTE_TRANSITION {
	ULONG Valid : 1;
	ULONG Write : 1;
	ULONG Owner : 1;
	ULONG WriteThrough : 1;
	ULONG CacheDisable : 1;
	ULONG Protection : 5;
	ULONG Prototype : 1;
	ULONG Transition : 1;
	ULONG PageFrameNumber : 20;
} MMPTE_TRANSITION;

typedef struct _MMPTE_PROTOTYPE {
	ULONG Valid : 1;
	ULONG ProtoAddressLow : 7;
	ULONG ReadOnly : 1;  // if set allow read only access.
	ULONG WhichPool : 1;
	ULONG Prototype : 1;
	ULONG ProtoAddressHigh : 21;
} MMPTE_PROTOTYPE;

typedef struct _MMPTE_HARDWARE {
	ULONG Valid : 1;
	ULONG Write : 1;       // UP version
	ULONG Owner : 1;
	ULONG WriteThrough : 1;
	ULONG CacheDisable : 1;
	ULONG Accessed : 1;
	ULONG Dirty : 1;
	ULONG LargePage : 1;
	ULONG Global : 1;
	ULONG CopyOnWrite : 1; // software field
	ULONG Prototype : 1;   // software field
	ULONG reserved : 1;    // software field
	ULONG PageFrameNumber : 20;
} MMPTE_HARDWARE, *PMMPTE_HARDWARE;

typedef struct _MMPTE {
	union  {
		ULONG Long;
		MMPTE_HARDWARE Hard;
		MMPTE_PROTOTYPE Proto;
		MMPTE_SOFTWARE Soft;
		MMPTE_TRANSITION Trans;
	} u;
} MMPTE, *PMMPTE;

typedef struct _MMPTE_SOFTWARE_PAE {
	ULONGLONG Valid : 1;
	ULONGLONG PageFileLow : 4;
	ULONGLONG Protection : 5;
	ULONGLONG Prototype : 1;
	ULONGLONG Transition : 1;
	ULONGLONG Unused : 20;
	ULONGLONG PageFileHigh : 32;
} MMPTE_SOFTWARE_PAE;

typedef struct _MMPTE_TRANSITION_PAE {
	ULONGLONG Valid : 1;
	ULONGLONG Write : 1;
	ULONGLONG Owner : 1;
	ULONGLONG WriteThrough : 1;
	ULONGLONG CacheDisable : 1;
	ULONGLONG Protection : 5;
	ULONGLONG Prototype : 1;
	ULONGLONG Transition : 1;
	ULONGLONG PageFrameNumber : 24;
	ULONGLONG Unused : 28;
} MMPTE_TRANSITION_PAE;

typedef struct _MMPTE_PROTOTYPE_PAE {
	ULONGLONG Valid : 1;
	ULONGLONG Unused0: 7;
	ULONGLONG ReadOnly : 1;  // if set allow read only access.  LWFIX: remove
	ULONGLONG Unused1: 1;
	ULONGLONG Prototype : 1;
	ULONGLONG Protection : 5;
	ULONGLONG Unused: 16;
	ULONGLONG ProtoAddress: 32;
} MMPTE_PROTOTYPE_PAE;

typedef struct _MMPTE_HARDWARE_PAE {
	ULONGLONG Valid : 1;
	ULONGLONG Write : 1;        // UP version
	ULONGLONG Owner : 1;
	ULONGLONG WriteThrough : 1;
	ULONGLONG CacheDisable : 1;
	ULONGLONG Accessed : 1;
	ULONGLONG Dirty : 1;
	ULONGLONG LargePage : 1;
	ULONGLONG Global : 1;
	ULONGLONG CopyOnWrite : 1; // software field
	ULONGLONG Prototype : 1;   // software field
	ULONGLONG reserved0 : 1;  // software field
	ULONGLONG PageFrameNumber : 24;
	ULONGLONG reserved1 : 28;  // software field
} MMPTE_HARDWARE_PAE, *PMMPTE_HARDWARE_PAE;

typedef struct _MMPTE_PAE
{
	union 
	{
		LARGE_INTEGER Long;
		MMPTE_HARDWARE_PAE Hard;
		MMPTE_PROTOTYPE_PAE Proto;
		MMPTE_SOFTWARE_PAE Soft;
		MMPTE_TRANSITION_PAE Trans;
	} u;
} MMPTE_PAE;
typedef MMPTE_PAE *PMMPTE_PAE;
#define PTE_BASE    0xC0000000
#define PDE_BASE    0xC0300000
#define PDE_BASE_PAE 0xc0600000

#define MiGetPteAddressPae(va)   ((PMMPTE_PAE)(PTE_BASE + ((((ULONG)(va)) >> 12) << 3)))
#define MiGetPdeAddressPae(va)   ((PMMPTE_PAE)(PDE_BASE_PAE + ((((ULONG)(va)) >> 21) << 3)))
#define MiGetPdeAddress(va)  ((MMPTE*)(((((ULONG)(va)) >> 22) << 2) + PDE_BASE))
#define MiGetPteAddress(va) ((MMPTE*)(((((ULONG)(va)) >> 12) << 2) + PTE_BASE))

typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	PDWORD ServiceTable;
	PULONG CounterTable;
	ULONG TableSize;
	PUCHAR ArgumentTable;
} SERVICE_DESCRIPTOR_TABLE,*PSERVICE_DESCRIPTOR_TABLE;

#pragma pack(pop)

extern PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
