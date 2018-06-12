namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions>
template <typename T, typename... TS>
struct M<TContext, TMaxSubstitutions>::_C final {
	using Client = T;
	using Fork	 = ForkT<Client>;
	using State  = _S<Client>;

	//----------------------------------------------------------------------

	template <unsigned TN, typename...>
	struct Sub;

	//----------------------------------------------------------------------

	template <unsigned TN, typename TI, typename... TR>
	struct Sub<TN, TI, TR...> {
		using Initial = typename WrapState<TI>::Type;
		using Remaining = Sub<TN + 1, TR...>;

		enum : unsigned {
			ProngIndex	 = TN,
			ReverseDepth = detail::Max<Initial::ReverseDepth, Remaining::ReverseDepth>::Value,
			DeepWidth	 = detail::Max<Initial::DeepWidth, Remaining::DeepWidth>::Value,
			StateCount	 = Initial::StateCount + Remaining::StateCount,
			ForkCount	 = Initial::ForkCount  + Remaining::ForkCount,
			ProngCount	 = Initial::ProngCount + Remaining::ProngCount,
		};

		Sub(StateRegistry& stateRegistry,
			const Index fork,
			Parents& stateParents,
			Parents& forkParents,
			ForkPointers& forkPointers);

		inline void wideForwardSubstitute(const unsigned prong, Control& control, Context& context);
		inline void wideSubstitute(const unsigned prong, Control& control, Context& context);

		inline void wideEnterInitial(Context& context);
		inline void wideEnter(const unsigned prong, Context& context);

		inline bool wideUpdateAndTransition(const unsigned prong, Control& control, Context& context);
		inline void wideUpdate(const unsigned prong, Context& context);

		template <typename TEvent>
		inline void wideReact(const unsigned prong, const TEvent& event, Control& control, Context& context);

		inline void wideLeave(const unsigned prong, Context& context);

		inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
		inline void wideRequestRemain();
		inline void wideRequestRestart();
		inline void wideRequestResume(const unsigned prong);
		inline void wideChangeToRequested(const unsigned prong, Context& context);

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	 = Initial::NameCount  + Remaining::NameCount,
		};

		void wideGetNames(const unsigned parent,
						  const unsigned depth,
						  StateInfos& stateInfos) const;

		void wideIsActive(const unsigned prong,
						  unsigned& index,
						  MachineStructure& structure) const;
	#endif

		Initial initial;
		Remaining remaining;
	};

	//----------------------------------------------------------------------

	template <unsigned TN, typename TI>
	struct Sub<TN, TI> {
		using Initial = typename WrapState<TI>::Type;

		enum : unsigned {
			ProngIndex	 = TN,
			ReverseDepth = Initial::ReverseDepth,
			DeepWidth	 = detail::Max<1, Initial::DeepWidth>::Value,
			StateCount	 = Initial::StateCount,
			ForkCount	 = Initial::ForkCount,
			ProngCount	 = Initial::ProngCount,
		};

		Sub(StateRegistry& stateRegistry,
			const Index fork,
			Parents& stateParents,
			Parents& forkParents,
			ForkPointers& forkPointers);

		inline void wideForwardSubstitute(const unsigned prong, Control& control, Context& context);
		inline void wideSubstitute(const unsigned prong, Control& control, Context& context);

		inline void wideEnterInitial(Context& context);
		inline void wideEnter(const unsigned prong, Context& context);

		inline bool wideUpdateAndTransition(const unsigned prong, Control& control, Context& context);
		inline void wideUpdate(const unsigned prong, Context& context);

		template <typename TEvent>
		inline void wideReact(const unsigned prong, const TEvent& event, Control& control, Context& context);

		inline void wideLeave(const unsigned prong, Context& context);

		inline void wideForwardRequest(const unsigned prong, const enum Transition::Type transition);
		inline void wideRequestRemain();
		inline void wideRequestRestart();
		inline void wideRequestResume(const unsigned prong);
		inline void wideChangeToRequested(const unsigned prong, Context& context);

	#ifdef HFSM_ENABLE_STRUCTURE_REPORT
		enum : unsigned {
			NameCount	 = Initial::NameCount,
		};

		void wideGetNames(const unsigned parent,
						  const unsigned depth,
						  StateInfos& stateInfos) const;

		void wideIsActive(const unsigned prong,
						  unsigned& index,
						  MachineStructure& structure) const;
	#endif

		Initial initial;
	};

	using SubStates = Sub<0, TS...>;

	//----------------------------------------------------------------------

	enum : unsigned {
		ReverseDepth = SubStates::ReverseDepth + 1,
		DeepWidth	 = SubStates::DeepWidth,
		StateCount	 = State::StateCount + SubStates::StateCount,
		ForkCount	 = SubStates::ForkCount + 1,
		ProngCount	 = SubStates::ProngCount + sizeof...(TS),
		Width		 = sizeof...(TS),
	};

	_C(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& stateParents,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardSubstitute(Control& control, Context& context);
	inline void deepSubstitute(Control& control, Context& context);

	inline void deepEnterInitial(Context& context);
	inline void deepEnter(Context& context);

	inline bool deepUpdateAndTransition(Control& control, Context& context);
	inline void deepUpdate(Context& context);

	template <typename TEvent>
	inline void deepReact(const TEvent& event, Control& control, Context& context);

	inline void deepLeave(Context& context);

	inline void deepForwardRequest(const enum Transition::Type transition);
	inline void deepRequestRemain();
	inline void deepRequestRestart();
	inline void deepRequestResume();
	inline void deepChangeToRequested(Context& context);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	enum : unsigned {
		NameCount	 = State::NameCount  + SubStates::NameCount,
	};

	void deepGetNames(const unsigned parent,
					  const enum StateInfo::RegionType region,
					  const unsigned depth,
					  StateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  unsigned& index,
					  MachineStructure& structure) const;
#endif
	Fork _fork;
	State _state;
	SubStates _subStates;

	HSFM_DEBUG_ONLY(const TypeInfo _type = TypeInfo::get<Client>());
};

////////////////////////////////////////////////////////////////////////////////

}

#include "machine_composite_methods.inl"
#include "machine_composite_sub_1.inl"
#include "machine_composite_sub_2.inl"
