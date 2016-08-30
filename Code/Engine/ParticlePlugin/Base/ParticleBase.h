#pragma once

#include <ParticlePlugin/Basics.h>
#include <CoreUtils/DataProcessing/Stream/Stream.h>
#include <ParticlePlugin/Declarations.h>
#include <ParticlePlugin/System/ParticleSystemInstance.h>

class ezStream;

template<class DerivedFrom, bool StreamInitializer>
class ezParticleBase : public DerivedFrom
{
  friend class ezParticleSystemInstance;

public:

  /// \brief Called after construction when m_pOwnerSystem is set and when properties have been set
  virtual void AfterPropertiesConfigured() {}
  virtual void CreateRequiredStreams() = 0;

  void Reset(ezParticleSystemInstance* pOwner)
  {
    m_pOwnerSystem = pOwner;
    m_StreamBinding.Clear();
  }

  const ezParticleSystemInstance* GetOwnerSystem() const
  {
    return m_pOwnerSystem;
  }

protected:
  void CreateStream(const char* szName, ezStream::DataType Type, ezStream** ppStream)
  {
    m_pOwnerSystem->CreateStream(szName, Type, ppStream, m_StreamBinding, !StreamInitializer);
  }

  virtual ezResult UpdateStreamBindings() final override
  {
    m_StreamBinding.UpdateBindings(m_pStreamGroup);
    return EZ_SUCCESS;
  }

  ezRandom& GetRNG() const
  {
    return m_pOwnerSystem->GetRNG();
  }

private:
  ezParticleSystemInstance* m_pOwnerSystem;
  ezParticleStreamBinding m_StreamBinding;
};