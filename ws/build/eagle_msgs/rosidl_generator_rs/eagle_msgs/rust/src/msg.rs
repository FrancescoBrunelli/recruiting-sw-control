#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to eagle_msgs__msg__Cone

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Cone {

    // This member is not documented.
    #[allow(missing_docs)]
    pub position: geometry_msgs::msg::Point,


    // This member is not documented.
    #[allow(missing_docs)]
    pub color: u8,

}

impl Cone {

    // This constant is not documented.
    #[allow(missing_docs)]
    pub const BLUE: u8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const YELLOW: u8 = 1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const ORANGE_SMALL: u8 = 2;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const ORANGE_BIG: u8 = 3;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const UNKNOWN: u8 = 4;

}


impl Default for Cone {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Cone::default())
  }
}

impl rosidl_runtime_rs::Message for Cone {
  type RmwMsg = super::msg::rmw::Cone;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        position: geometry_msgs::msg::Point::into_rmw_message(std::borrow::Cow::Owned(msg.position)).into_owned(),
        color: msg.color,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        position: geometry_msgs::msg::Point::into_rmw_message(std::borrow::Cow::Borrowed(&msg.position)).into_owned(),
      color: msg.color,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      position: geometry_msgs::msg::Point::from_rmw_message(msg.position),
      color: msg.color,
    }
  }
}


// Corresponds to eagle_msgs__msg__ConeArray
/// Cones sorted by ascending distance from the vehicle.
/// Frame is given in header.frame_id (base_link by default).

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConeArray {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub cones: Vec<super::msg::Cone>,

}



impl Default for ConeArray {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ConeArray::default())
  }
}

impl rosidl_runtime_rs::Message for ConeArray {
  type RmwMsg = super::msg::rmw::ConeArray;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        cones: msg.cones
          .into_iter()
          .map(|elem| super::msg::Cone::into_rmw_message(std::borrow::Cow::Owned(elem)).into_owned())
          .collect(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        cones: msg.cones
          .iter()
          .map(|elem| super::msg::Cone::into_rmw_message(std::borrow::Cow::Borrowed(elem)).into_owned())
          .collect(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      cones: msg.cones
          .into_iter()
          .map(super::msg::Cone::from_rmw_message)
          .collect(),
    }
  }
}


