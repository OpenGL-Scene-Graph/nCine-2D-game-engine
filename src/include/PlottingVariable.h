#ifndef CLASS_NCINE_PLOTTINGVARIABLE
#define CLASS_NCINE_PLOTTINGVARIABLE

#include "Color.h"
#include "ProfileVariable.h"
#include "DrawableNode.h"
#include "RenderCommand.h"
#include "RenderQueue.h"

namespace ncine {

/// A class to group and wrap all the information needed to plot a variable
class PlottingVariable
{
  public:
	PlottingVariable(unsigned int numValues, float rejectDelay, const Matrix4x4f& worldMatrix);
	virtual ~PlottingVariable();

	/// Returns the number of value for the variable
	inline unsigned int numValues() const { return variable_.numValues(); }
	/// Returns the normalized mathematic mean of the variable
	inline float normMean() const { return variable_.normMean(); }
	/// Returns the graph color
	inline const Color& graphColor() const { return graphColor_; }
	/// Returns the mean line color
	inline const Color& meanColor() const { return meanColor_; }
	/// Returns a constant pointer to the vertices buffer
	inline const float* vertices() const { return vertices_; }
	/// Returns the pointer to the vertices buffer
	inline float* vertices() { return vertices_; }
	/// Returns the pointer to the variable
	inline const ProfileVariable* variable() const { return &variable_; }
	/// Returns the state of the mean drawing flag
	inline bool shouldPlotMean() const { return shouldPlotMean_; }

	/// Adds a value to the variable
	bool addValue(float value) { return variable_.addValue(value); }
	/// Sets the graph color
	inline void setGraphColor(Color graphColor) { graphColor_ = graphColor; }
	/// Sets the mean line color
	inline void setMeanColor(Color meanColor) { meanColor_ = meanColor; }
	/// Sets the mean drawing flag state
	inline void setPlotMean(bool enabled) { shouldPlotMean_ = enabled; }

	inline virtual void draw(RenderQueue& renderQueue)
	{
		updateRenderCommand();
		renderQueue.addCommand(&valuesCmd_);
	}
	inline virtual void drawMean(RenderQueue& renderQueue)
	{
		updateMeanRenderCommand();
		renderQueue.addCommand(&meanCmd_);
	}

  protected:
	/// Mean drawing flag
	bool shouldPlotMean_;
	/// Graph color
	Color graphColor_;
	/// Mean line color
	Color meanColor_;
	/// The variable to be plotted
	ProfileVariable variable_;
	/// The vertices buffer
	float *vertices_;
	/// A reference to the world matrix of the profile plotter
	const Matrix4x4f& worldMatrix_;

	/// The command used to render variable values
	RenderCommand valuesCmd_;
	/// The command used to render the variable mean
	RenderCommand meanCmd_;

	/// Updates the values rendering command
	virtual void updateRenderCommand() = 0;
	/// Updates the mean rendering command
	virtual void updateMeanRenderCommand() = 0;

  private:
	/// Private copy constructor
	PlottingVariable(const PlottingVariable&);
	/// Private assignment operator
	PlottingVariable& operator=(const PlottingVariable&);
};

}

#endif